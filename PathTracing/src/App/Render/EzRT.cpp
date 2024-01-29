#include  "EzRT.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <ctime>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "App/HDRLoader.h"

#define INF 114514.0

using namespace glm;

// ----------------------------------------------------------------------------- //

// ���������ʶ���
struct Material {
    vec3 emissive = vec3(0, 0, 0);  // ��Ϊ��Դʱ�ķ�����ɫ
    vec3 baseColor = vec3(1, 1, 1);
    float subsurface = 0.0;
    float metallic = 0.0;
    float specular = 0.0;
    float specularTint = 0.0;
    float roughness = 0.0;
    float anisotropic = 0.0;
    float sheen = 0.0;
    float sheenTint = 0.0;
    float clearcoat = 0.0;
    float clearcoatGloss = 0.0;
    float IOR = 1.0;
    float transmission = 0.0;
};

// �����ζ���
struct Triangle {
    vec3 p1, p2, p3;    // ��������
    vec3 n1, n2, n3;    // ���㷨��
    Material material;  // ����
};

// BVH ���ڵ�
struct BVHNode {
    int left, right;    // ������������
    int n, index;       // Ҷ�ӽڵ���Ϣ               
    vec3 AA, BB;        // ��ײ��
};

// ----------------------------------------------------------------------------- //

struct Triangle_encoded {
    vec3 p1, p2, p3;    // ��������
    vec3 n1, n2, n3;    // ���㷨��
    vec3 emissive;      // �Է������
    vec3 baseColor;     // ��ɫ
    vec3 param1;        // (subsurface, metallic, specular)
    vec3 param2;        // (specularTint, roughness, anisotropic)
    vec3 param3;        // (sheen, sheenTint, clearcoat)
    vec3 param4;        // (clearcoatGloss, IOR, transmission)
};

struct BVHNode_encoded {
    vec3 childs;        // (left, right, ����)
    vec3 leafInfo;      // (n, index, ����)
    vec3 AA, BB;
};

// ----------------------------------------------------------------------------- //

class RenderPass {
public:
    GLuint FBO = 0;
    GLuint vao, vbo;
    std::vector<GLuint> colorAttachments;
    GLuint program;
    int width = 512;
    int height = 512;
    void bindData(bool finalPass = false) {
        if (!finalPass) glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        std::vector<vec3> square = { vec3(-1, -1, 0), vec3(1, -1, 0), vec3(-1, 1, 0), vec3(1, 1, 0), vec3(-1, 1, 0), vec3(1, -1, 0) };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * square.size(), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * square.size(), &square[0]);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);   // layout (location = 0) 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        // ���� finalPass ������֡�������ɫ����
        if (!finalPass) {
            std::vector<GLuint> attachments;
            for (int i = 0; i < colorAttachments.size(); i++) {
                glBindTexture(GL_TEXTURE_2D, colorAttachments[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorAttachments[i], 0);// ����ɫ����󶨵� i ����ɫ����
                attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
            }
            glDrawBuffers(attachments.size(), &attachments[0]);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void draw(std::vector<GLuint> texPassArray = {}) {
        glUseProgram(program);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glBindVertexArray(vao);
        // ����һ֡��֡������ɫ����
        for (int i = 0; i < texPassArray.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, texPassArray[i]);
            std::string uName = "texPass" + std::to_string(i);
            glUniform1i(glGetUniformLocation(program, uName.c_str()), i);
        }
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUseProgram(0);
    }
};

// ----------------------------------------------------------------------------- //

GLuint trianglesTextureBuffer;
GLuint nodesTextureBuffer;
GLuint lastFrame;
GLuint hdrMap;

RenderPass pass1;
RenderPass pass2;
RenderPass pass3;

// �������
float upAngle = 0.0;
float rotatAngle = 0.0;
float r = 4.0;

// ----------------------------------------------------------------------------- //

// ������������������ -- �ȽϺ���
bool cmpx(const Triangle& t1, const Triangle& t2) {
    vec3 center1 = (t1.p1 + t1.p2 + t1.p3) / vec3(3, 3, 3);
    vec3 center2 = (t2.p1 + t2.p2 + t2.p3) / vec3(3, 3, 3);
    return center1.x < center2.x;
}
bool cmpy(const Triangle& t1, const Triangle& t2) {
    vec3 center1 = (t1.p1 + t1.p2 + t1.p3) / vec3(3, 3, 3);
    vec3 center2 = (t2.p1 + t2.p2 + t2.p3) / vec3(3, 3, 3);
    return center1.y < center2.y;
}
bool cmpz(const Triangle& t1, const Triangle& t2) {
    vec3 center1 = (t1.p1 + t1.p2 + t1.p3) / vec3(3, 3, 3);
    vec3 center2 = (t2.p1 + t2.p2 + t2.p3) / vec3(3, 3, 3);
    return center1.z < center2.z;
}

// ----------------------------------------------------------------------------- //

// ��ȡ�ļ����ҷ���һ�����ַ�����ʾ�ļ�����
std::string readShaderFile(std::string filepath) {
    std::string res, line;
    std::ifstream fin(filepath);
    if (!fin.is_open())
    {
        std::cout << "�ļ� " << filepath << " ��ʧ��" << std::endl;
        exit(-1);
    }
    while (std::getline(fin, line))
    {
        res += line + '\n';
    }
    fin.close();
    return res;
}

GLuint getTextureRGB32F(int width, int height) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return tex;
}

// ��ȡ��ɫ������
GLuint getShaderProgram(std::string fshader, std::string vshader) {
    // ��ȡshaderԴ�ļ�
    std::string vSource = readShaderFile(vshader);
    std::string fSource = readShaderFile(fshader);
    const char* vpointer = vSource.c_str();
    const char* fpointer = fSource.c_str();

    // �ݴ�
    GLint success;
    GLchar infoLog[512];

    // ���������붥����ɫ��
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)(&vpointer), NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);   // ������
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "������ɫ���������\n" << infoLog << std::endl;
        exit(-1);
    }

    // �������ұ���Ƭ����ɫ��
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)(&fpointer), NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);   // ������
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Ƭ����ɫ���������\n" << infoLog << std::endl;
        exit(-1);
    }

    // ����������ɫ����program����
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // ɾ����ɫ������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// ----------------------------------------------------------------------------- //

// ģ�ͱ任����
mat4 getTransformMatrix(vec3 rotateCtrl, vec3 translateCtrl, vec3 scaleCtrl) {
    glm::mat4 unit(    // ��λ����
        glm::vec4(1, 0, 0, 0),
        glm::vec4(0, 1, 0, 0),
        glm::vec4(0, 0, 1, 0),
        glm::vec4(0, 0, 0, 1)
    );
    mat4 scale = glm::scale(unit, scaleCtrl);
    mat4 translate = glm::translate(unit, translateCtrl);
    mat4 rotate = unit;
    rotate = glm::rotate(rotate, glm::radians(rotateCtrl.x), glm::vec3(1, 0, 0));
    rotate = glm::rotate(rotate, glm::radians(rotateCtrl.y), glm::vec3(0, 1, 0));
    rotate = glm::rotate(rotate, glm::radians(rotateCtrl.z), glm::vec3(0, 0, 1));

    mat4 model = translate * rotate * scale;
    return model;
}

// ��ȡ obj
void readObj(std::string filepath, std::vector<Triangle>& triangles, Material material, mat4 trans, bool smoothNormal) {

    // ����λ�ã�����
    std::vector<vec3> vertices;
    std::vector<GLuint> indices;

    // ���ļ���
    std::ifstream fin(filepath);
    std::string line;
    if (!fin.is_open()) {
        std::cout << "�ļ� " << filepath << " ��ʧ��" << std::endl;
        exit(-1);
    }

    // ���� AABB �У���һ��ģ�ʹ�С
    float maxx = -11451419.19;
    float maxy = -11451419.19;
    float maxz = -11451419.19;
    float minx = 11451419.19;
    float miny = 11451419.19;
    float minz = 11451419.19;

    // ���ж�ȡ
    while (std::getline(fin, line)) {
        std::istringstream sin(line);   // ��һ�е�������Ϊ string stream �������Ҷ�ȡ
        std::string type;
        GLfloat x, y, z;
        int v0, v1, v2;
        int vn0, vn1, vn2;
        int vt0, vt1, vt2;
        char slash;

        // ͳ��б����Ŀ���ò�ͬ��ʽ��ȡ
        int slashCnt = 0;
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == '/') slashCnt++;
        }

        // ��ȡobj�ļ�
        sin >> type;
        if (type == "v") {
            sin >> x >> y >> z;
            vertices.push_back(vec3(x, y, z));
            maxx = max(maxx, x); maxy = max(maxx, y); maxz = max(maxx, z);
            minx = min(minx, x); miny = min(minx, y); minz = min(minx, z);
        }
        if (type == "f") {
            if (slashCnt == 6) {
                sin >> v0 >> slash >> vt0 >> slash >> vn0;
                sin >> v1 >> slash >> vt1 >> slash >> vn1;
                sin >> v2 >> slash >> vt2 >> slash >> vn2;
            }
            else if (slashCnt == 3) {
                sin >> v0 >> slash >> vt0;
                sin >> v1 >> slash >> vt1;
                sin >> v2 >> slash >> vt2;
            }
            else {
                sin >> v0 >> v1 >> v2;
            }
            indices.push_back(v0 - 1);
            indices.push_back(v1 - 1);
            indices.push_back(v2 - 1);
        }
    }

    // ģ�ʹ�С��һ��
    float lenx = maxx - minx;
    float leny = maxy - miny;
    float lenz = maxz - minz;
    float maxaxis = max(lenx, max(leny, lenz));
    for (auto& v : vertices) {
        v.x /= maxaxis;
        v.y /= maxaxis;
        v.z /= maxaxis;
    }

    // ͨ�������������任
    for (auto& v : vertices) {
        vec4 vv = vec4(v.x, v.y, v.z, 1);
        vv = trans * vv;
        v = vec3(vv.x, vv.y, vv.z);
    }

    // ���ɷ���
    std::vector<vec3> normals(vertices.size(), vec3(0, 0, 0));
    for (int i = 0; i < indices.size(); i += 3) {
        vec3 p1 = vertices[indices[i]];
        vec3 p2 = vertices[indices[i + 1]];
        vec3 p3 = vertices[indices[i + 2]];
        vec3 n = normalize(cross(p2 - p1, p3 - p1));
        normals[indices[i]] += n;
        normals[indices[i + 1]] += n;
        normals[indices[i + 2]] += n;
    }

    // ���� Triangle ��������
    int offset = triangles.size();  // ��������
    triangles.resize(offset + indices.size() / 3);
    for (int i = 0; i < indices.size(); i += 3) {
        Triangle& t = triangles[offset + i / 3];
        // ����������
        t.p1 = vertices[indices[i]];
        t.p2 = vertices[indices[i + 1]];
        t.p3 = vertices[indices[i + 2]];
        if (!smoothNormal) {
            vec3 n = normalize(cross(t.p2 - t.p1, t.p3 - t.p1));
            t.n1 = n; t.n2 = n; t.n3 = n;
        }
        else {
            t.n1 = normalize(normals[indices[i]]);
            t.n2 = normalize(normals[indices[i + 1]]);
            t.n3 = normalize(normals[indices[i + 2]]);
        }

        // ������
        t.material = material;
    }
}

// ���� BVH
int buildBVH(std::vector<Triangle>& triangles, std::vector<BVHNode>& nodes, int l, int r, int n) {
    if (l > r) return 0;

    // ע��
    // �˴�����ͨ��ָ�룬���õȷ�ʽ������������ nodes[id] ������
    // ��Ϊ std::vector<> ����ʱ�´����������ڴ棬��ô��ַ�͸ı���
    // ��ָ�룬���þ�ָ��ԭ�����ڴ棬���Իᷢ������
    nodes.push_back(BVHNode());
    int id = nodes.size() - 1;   // ע�⣺ �ȱ�������
    nodes[id].left = nodes[id].right = nodes[id].n = nodes[id].index = 0;
    nodes[id].AA = vec3(1145141919, 1145141919, 1145141919);
    nodes[id].BB = vec3(-1145141919, -1145141919, -1145141919);

    // ���� AABB
    for (int i = l; i <= r; i++) {
        // ��С�� AA
        float minx = min(triangles[i].p1.x, min(triangles[i].p2.x, triangles[i].p3.x));
        float miny = min(triangles[i].p1.y, min(triangles[i].p2.y, triangles[i].p3.y));
        float minz = min(triangles[i].p1.z, min(triangles[i].p2.z, triangles[i].p3.z));
        nodes[id].AA.x = min(nodes[id].AA.x, minx);
        nodes[id].AA.y = min(nodes[id].AA.y, miny);
        nodes[id].AA.z = min(nodes[id].AA.z, minz);
        // ���� BB
        float maxx = max(triangles[i].p1.x, max(triangles[i].p2.x, triangles[i].p3.x));
        float maxy = max(triangles[i].p1.y, max(triangles[i].p2.y, triangles[i].p3.y));
        float maxz = max(triangles[i].p1.z, max(triangles[i].p2.z, triangles[i].p3.z));
        nodes[id].BB.x = max(nodes[id].BB.x, maxx);
        nodes[id].BB.y = max(nodes[id].BB.y, maxy);
        nodes[id].BB.z = max(nodes[id].BB.z, maxz);
    }

    // ������ n �������� ����Ҷ�ӽڵ�
    if ((r - l + 1) <= n) {
        nodes[id].n = r - l + 1;
        nodes[id].index = l;
        return id;
    }

    // ����ݹ齨��
    float lenx = nodes[id].BB.x - nodes[id].AA.x;
    float leny = nodes[id].BB.y - nodes[id].AA.y;
    float lenz = nodes[id].BB.z - nodes[id].AA.z;
    // �� x ����
    if (lenx >= leny && lenx >= lenz)
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpx);
    // �� y ����
    if (leny >= lenx && leny >= lenz)
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpy);
    // �� z ����
    if (lenz >= lenx && lenz >= leny)
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpz);
    // �ݹ�
    int mid = (l + r) / 2;
    int left = buildBVH(triangles, nodes, l, mid, n);
    int right = buildBVH(triangles, nodes, mid + 1, r, n);

    nodes[id].left = left;
    nodes[id].right = right;

    return id;
}

// SAH �Ż����� BVH
int buildBVHwithSAH(std::vector<Triangle>& triangles, std::vector<BVHNode>& nodes, int l, int r, int n) {
    if (l > r) return 0;

    nodes.push_back(BVHNode());
    int id = nodes.size() - 1;
    nodes[id].left = nodes[id].right = nodes[id].n = nodes[id].index = 0;
    nodes[id].AA = vec3(1145141919, 1145141919, 1145141919);
    nodes[id].BB = vec3(-1145141919, -1145141919, -1145141919);

    // ���� AABB
    for (int i = l; i <= r; i++) {
        // ��С�� AA
        float minx = min(triangles[i].p1.x, min(triangles[i].p2.x, triangles[i].p3.x));
        float miny = min(triangles[i].p1.y, min(triangles[i].p2.y, triangles[i].p3.y));
        float minz = min(triangles[i].p1.z, min(triangles[i].p2.z, triangles[i].p3.z));
        nodes[id].AA.x = min(nodes[id].AA.x, minx);
        nodes[id].AA.y = min(nodes[id].AA.y, miny);
        nodes[id].AA.z = min(nodes[id].AA.z, minz);
        // ���� BB
        float maxx = max(triangles[i].p1.x, max(triangles[i].p2.x, triangles[i].p3.x));
        float maxy = max(triangles[i].p1.y, max(triangles[i].p2.y, triangles[i].p3.y));
        float maxz = max(triangles[i].p1.z, max(triangles[i].p2.z, triangles[i].p3.z));
        nodes[id].BB.x = max(nodes[id].BB.x, maxx);
        nodes[id].BB.y = max(nodes[id].BB.y, maxy);
        nodes[id].BB.z = max(nodes[id].BB.z, maxz);
    }

    // ������ n �������� ����Ҷ�ӽڵ�
    if ((r - l + 1) <= n) {
        nodes[id].n = r - l + 1;
        nodes[id].index = l;
        return id;
    }

    // ����ݹ齨��
    float Cost = INF;
    int Axis = 0;
    int Split = (l + r) / 2;
    for (int axis = 0; axis < 3; axis++) {
        // �ֱ� x��y��z ������
        if (axis == 0) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpx);
        if (axis == 1) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpy);
        if (axis == 2) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpz);

        // leftMax[i]: [l, i] ������ xyz ֵ
        // leftMin[i]: [l, i] ����С�� xyz ֵ
        std::vector<vec3> leftMax(r - l + 1, vec3(-INF, -INF, -INF));
        std::vector<vec3> leftMin(r - l + 1, vec3(INF, INF, INF));
        // ����ǰ׺ ע�� i-l �Զ��뵽�±� 0
        for (int i = l; i <= r; i++) {
            Triangle& t = triangles[i];
            int bias = (i == l) ? 0 : 1;  // ��һ��Ԫ�����⴦��

            leftMax[i - l].x = max(leftMax[i - l - bias].x, max(t.p1.x, max(t.p2.x, t.p3.x)));
            leftMax[i - l].y = max(leftMax[i - l - bias].y, max(t.p1.y, max(t.p2.y, t.p3.y)));
            leftMax[i - l].z = max(leftMax[i - l - bias].z, max(t.p1.z, max(t.p2.z, t.p3.z)));

            leftMin[i - l].x = min(leftMin[i - l - bias].x, min(t.p1.x, min(t.p2.x, t.p3.x)));
            leftMin[i - l].y = min(leftMin[i - l - bias].y, min(t.p1.y, min(t.p2.y, t.p3.y)));
            leftMin[i - l].z = min(leftMin[i - l - bias].z, min(t.p1.z, min(t.p2.z, t.p3.z)));
        }

        // rightMax[i]: [i, r] ������ xyz ֵ
        // rightMin[i]: [i, r] ����С�� xyz ֵ
        std::vector<vec3> rightMax(r - l + 1, vec3(-INF, -INF, -INF));
        std::vector<vec3> rightMin(r - l + 1, vec3(INF, INF, INF));
        // �����׺ ע�� i-l �Զ��뵽�±� 0
        for (int i = r; i >= l; i--) {
            Triangle& t = triangles[i];
            int bias = (i == r) ? 0 : 1;  // ��һ��Ԫ�����⴦��

            rightMax[i - l].x = max(rightMax[i - l + bias].x, max(t.p1.x, max(t.p2.x, t.p3.x)));
            rightMax[i - l].y = max(rightMax[i - l + bias].y, max(t.p1.y, max(t.p2.y, t.p3.y)));
            rightMax[i - l].z = max(rightMax[i - l + bias].z, max(t.p1.z, max(t.p2.z, t.p3.z)));

            rightMin[i - l].x = min(rightMin[i - l + bias].x, min(t.p1.x, min(t.p2.x, t.p3.x)));
            rightMin[i - l].y = min(rightMin[i - l + bias].y, min(t.p1.y, min(t.p2.y, t.p3.y)));
            rightMin[i - l].z = min(rightMin[i - l + bias].z, min(t.p1.z, min(t.p2.z, t.p3.z)));
        }

        // ����Ѱ�ҷָ�
        float cost = INF;
        int split = l;
        for (int i = l; i <= r - 1; i++) {
            float lenx, leny, lenz;
            // ��� [l, i]
            vec3 leftAA = leftMin[i - l];
            vec3 leftBB = leftMax[i - l];
            lenx = leftBB.x - leftAA.x;
            leny = leftBB.y - leftAA.y;
            lenz = leftBB.z - leftAA.z;
            float leftS = 2.0 * ((lenx * leny) + (lenx * lenz) + (leny * lenz));
            float leftCost = leftS * (i - l + 1);

            // �Ҳ� [i+1, r]
            vec3 rightAA = rightMin[i + 1 - l];
            vec3 rightBB = rightMax[i + 1 - l];
            lenx = rightBB.x - rightAA.x;
            leny = rightBB.y - rightAA.y;
            lenz = rightBB.z - rightAA.z;
            float rightS = 2.0 * ((lenx * leny) + (lenx * lenz) + (leny * lenz));
            float rightCost = rightS * (r - i);

            // ��¼ÿ���ָ����С��
            float totalCost = leftCost + rightCost;
            if (totalCost < cost) {
                cost = totalCost;
                split = i;
            }
        }
        // ��¼ÿ�������Ѵ�
        if (cost < Cost) {
            Cost = cost;
            Axis = axis;
            Split = split;
        }
    }

    // �������ָ�
    if (Axis == 0) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpx);
    if (Axis == 1) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpy);
    if (Axis == 2) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpz);

    // �ݹ�
    int left = buildBVHwithSAH(triangles, nodes, l, Split, n);
    int right = buildBVHwithSAH(triangles, nodes, Split + 1, r, n);

    nodes[id].left = left;
    nodes[id].right = right;

    return id;
}

// ----------------------------------------------------------------------------- //

// ����
clock_t t1, t2;
double dt, fps;
unsigned int frameCounter = 0;
void display(Camera& camera) {

    // ֡��ʱ
    t2 = clock();
    dt = (double)(t2 - t1) / CLOCKS_PER_SEC;
    fps = 1.0 / dt;
    std::cout << "\r";
    std::cout << std::fixed << std::setprecision(2) << "FPS : " << fps << "    ��������: " << frameCounter;
    t1 = t2;

    // �������
    vec3 eye = vec3(-sin(radians(rotatAngle)) * cos(radians(upAngle)), sin(radians(upAngle)), cos(radians(rotatAngle)) * cos(radians(upAngle)));
    eye.x *= r; eye.y *= r; eye.z *= r;
    mat4 cameraRotate = lookAt(eye, vec3(0, 0, 0), vec3(0, 1, 0));  // ���ע����ԭ��
    cameraRotate = inverse(cameraRotate);   // lookat ������󽫹��߷������ת��

    // �� uniform �� pass1
    glUseProgram(pass1.program);
    //glUniform3fv(glGetUniformLocation(pass1.program, "eye"), 1, value_ptr(eye));
    glUniform3fv(glGetUniformLocation(pass1.program, "eye"), 1, glm::value_ptr(camera.GetPosition()));

    glUniformMatrix4fv(glGetUniformLocation(pass1.program, "cameraRotate"), 1, GL_FALSE, value_ptr(cameraRotate));
    glUniform1ui(glGetUniformLocation(pass1.program, "frameCounter"), frameCounter++);// �������������������

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_BUFFER, trianglesTextureBuffer);
    glUniform1i(glGetUniformLocation(pass1.program, "triangles"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_BUFFER, nodesTextureBuffer);
    glUniform1i(glGetUniformLocation(pass1.program, "nodes"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, lastFrame);
    glUniform1i(glGetUniformLocation(pass1.program, "lastFrame"), 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, hdrMap);
    glUniform1i(glGetUniformLocation(pass1.program, "hdrMap"), 3);

    // ����
    pass1.draw();
    pass2.draw(pass1.colorAttachments);
    pass3.draw(pass2.colorAttachments);

}



// ----------------------------------------------------------------------------- //
EzRT::EzRT()
{

    // ----------------------------------------------------------------------------- //

    // scene config
    std::vector<Triangle> triangles;

    Material m;
    m.baseColor = vec3(1, 1, 1);
    readObj("./assets/models/Stanford Bunny.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0.3, -1.6, 0), vec3(1.5, 1.5, 1.5)), true);
    //readObj("models/room.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0.0, -2.5, 0), vec3(10, 10, 10)), true);

    m.baseColor = vec3(0.725, 0.71, 0.68);
    readObj("./assets/models/quad.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0, -1.4, 0), vec3(18.83, 0.01, 18.83)), false);

    m.baseColor = vec3(1, 1, 1);
    m.emissive = vec3(30, 20, 10);
    //readObj("models/quad.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0.0, 1.38, -0.0), vec3(0.7, 0.01, 0.7)), false);
    readObj("./assets/models/sphere.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0.0, 0.9, -0.0), vec3(1, 1, 1)), false);

    int nTriangles = triangles.size();
    std::cout << "ģ�Ͷ�ȡ���: �� " << nTriangles << " ��������" << std::endl;

    // ���� bvh
    BVHNode testNode;
    testNode.left = 255;
    testNode.right = 128;
    testNode.n = 30;
    testNode.AA = vec3(1, 1, 0);
    testNode.BB = vec3(0, 1, 0);
    std::vector<BVHNode> nodes{ testNode };
    //buildBVH(triangles, nodes, 0, triangles.size() - 1, 8);
    buildBVHwithSAH(triangles, nodes, 0, triangles.size() - 1, 8);
    int nNodes = nodes.size();
    std::cout << "BVH �������: �� " << nNodes << " ���ڵ�" << std::endl;

    // ���� ������, ����
    std::vector<Triangle_encoded> triangles_encoded(nTriangles);
    for (int i = 0; i < nTriangles; i++) {
        Triangle& t = triangles[i];
        Material& m = t.material;
        // ����λ��
        triangles_encoded[i].p1 = t.p1;
        triangles_encoded[i].p2 = t.p2;
        triangles_encoded[i].p3 = t.p3;
        // ���㷨��
        triangles_encoded[i].n1 = t.n1;
        triangles_encoded[i].n2 = t.n2;
        triangles_encoded[i].n3 = t.n3;
        // ����
        triangles_encoded[i].emissive = m.emissive;
        triangles_encoded[i].baseColor = m.baseColor;
        triangles_encoded[i].param1 = vec3(m.subsurface, m.metallic, m.specular);
        triangles_encoded[i].param2 = vec3(m.specularTint, m.roughness, m.anisotropic);
        triangles_encoded[i].param3 = vec3(m.sheen, m.sheenTint, m.clearcoat);
        triangles_encoded[i].param4 = vec3(m.clearcoatGloss, m.IOR, m.transmission);
    }

    // ���� BVHNode, aabb
    std::vector<BVHNode_encoded> nodes_encoded(nNodes);
    for (int i = 0; i < nNodes; i++) {
        nodes_encoded[i].childs = vec3(nodes[i].left, nodes[i].right, 0);
        nodes_encoded[i].leafInfo = vec3(nodes[i].n, nodes[i].index, 0);
        nodes_encoded[i].AA = nodes[i].AA;
        nodes_encoded[i].BB = nodes[i].BB;
    }

    // ----------------------------------------------------------------------------- //

    // ��������

    // ����������
    GLuint tbo0;
    glGenBuffers(1, &tbo0);
    glBindBuffer(GL_TEXTURE_BUFFER, tbo0);
    glBufferData(GL_TEXTURE_BUFFER, triangles_encoded.size() * sizeof(Triangle_encoded), &triangles_encoded[0], GL_STATIC_DRAW);
    glGenTextures(1, &trianglesTextureBuffer);
    glBindTexture(GL_TEXTURE_BUFFER, trianglesTextureBuffer);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, tbo0);

    // BVHNode ����
    GLuint tbo1;
    glGenBuffers(1, &tbo1);
    glBindBuffer(GL_TEXTURE_BUFFER, tbo1);
    glBufferData(GL_TEXTURE_BUFFER, nodes_encoded.size() * sizeof(BVHNode_encoded), &nodes_encoded[0], GL_STATIC_DRAW);
    glGenTextures(1, &nodesTextureBuffer);
    glBindTexture(GL_TEXTURE_BUFFER, nodesTextureBuffer);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, tbo1);

    // hdr ȫ��ͼ
    HDRLoaderResult hdrRes;
    bool r = HDRLoader::load("./assets/textures/HDR/sunset.hdr", hdrRes);
    hdrMap = getTextureRGB32F(hdrRes.width, hdrRes.height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, hdrRes.width, hdrRes.height, 0, GL_RGB, GL_FLOAT, hdrRes.cols);

    // ----------------------------------------------------------------------------- //

    // ��������

    pass1.program = getShaderProgram("./assets/shaders/fshader.fsh", "./assets/shaders/vshader.vsh");
    //pass1.width = pass1.height = 256;
    pass1.colorAttachments.push_back(getTextureRGB32F(pass1.width, pass1.height));
    pass1.colorAttachments.push_back(getTextureRGB32F(pass1.width, pass1.height));
    pass1.colorAttachments.push_back(getTextureRGB32F(pass1.width, pass1.height));
    pass1.bindData();

    glUseProgram(pass1.program);
    glUniform1i(glGetUniformLocation(pass1.program, "nTriangles"), triangles.size());
    glUniform1i(glGetUniformLocation(pass1.program, "nNodes"), nodes.size());
    glUniform1i(glGetUniformLocation(pass1.program, "width"), pass1.width);
    glUniform1i(glGetUniformLocation(pass1.program, "height"), pass1.height);
    glUseProgram(0);

    pass2.program = getShaderProgram("./assets/shaders/pass2.fsh", "./assets/shaders/vshader.vsh");
    lastFrame = getTextureRGB32F(pass2.width, pass2.height);
    pass2.colorAttachments.push_back(lastFrame);
    pass2.bindData();

    pass3.program = getShaderProgram("./assets/shaders/pass3.fsh", "./assets/shaders/vshader.vsh");
    pass3.bindData(true);


    // ----------------------------------------------------------------------------- //

    std::cout << "��ʼ..." << std::endl << std::endl;

    glEnable(GL_DEPTH_TEST);  // ������Ȳ���
    glClearColor(0.0, 0.0, 0.0, 1.0);   // ������ɫ -- ��

}

void EzRT::Render(Camera& camera)
{
    display(camera);
}