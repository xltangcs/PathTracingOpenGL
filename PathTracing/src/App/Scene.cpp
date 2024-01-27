#include "Scene.h"

#include <algorithm>
#include <iostream>

const float INF = 0x3f3f3f3f;

// ----------------------------------------------------------------------------- //

// 按照三角形中心排序 -- 比较函数
bool cmpx(const Triangle& t1, const Triangle& t2) {
    glm::vec3 center1 = (t1.position[0] + t1.position[1] + t1.position[2]) / glm::vec3(3, 3, 3);
    glm::vec3 center2 = (t2.position[0] + t2.position[1] + t2.position[2]) / glm::vec3(3, 3, 3);
    return center1.x < center2.x;
}
bool cmpy(const Triangle& t1, const Triangle& t2) {
    glm::vec3 center1 = (t1.position[0] + t1.position[1] + t1.position[2]) / glm::vec3(3, 3, 3);
    glm::vec3 center2 = (t2.position[0] + t2.position[1] + t2.position[2]) / glm::vec3(3, 3, 3);
    return center1.y < center2.y;
}
bool cmpz(const Triangle& t1, const Triangle& t2) {
    glm::vec3 center1 = (t1.position[0] + t1.position[1] + t1.position[2]) / glm::vec3(3, 3, 3);
    glm::vec3 center2 = (t2.position[0] + t2.position[1] + t2.position[2]) / glm::vec3(3, 3, 3);
    return center1.z < center2.z;
}

int Scene::AddModel(const std::string& filename)
{
    int id = -1;
    // Check if mesh was already loaded
    for (int i = 0; i < m_Models.size(); i++)
        if (m_Models[i]->Name == filename)
            return i;

    id = m_Models.size();
    Model* model = new Model;

    printf("Loading model %s\n", filename.c_str());
    if (model->LoadFromFile(filename))
    {
        m_Models.push_back(model);

        for (size_t i = 0; i < model->Triangles.size(); i++)
        {
            auto& t = model->Triangles[i];
            Material& m = t.material;
            m.baseColor = glm::vec3(0.2, 0.4, 0.4);

            TriangleEncoded te;
            te.p1 = t.position[0];
            te.p2 = t.position[1];
            te.p3 = t.position[2];
            te.n3 = t.normal[0];
            te.n3 = t.normal[1];
            te.n3 = t.normal[2];
            te.emissive = m.emissive;
            te.baseColor = m.baseColor;
            te.param1 = glm::vec3(m.subsurface, m.metallic, m.specular);
            te.param2 = glm::vec3(m.specularTint, m.roughness, m.anisotropic);
            te.param3 = glm::vec3(m.sheen, m.sheenTint, m.clearcoat);
            te.param4 = glm::vec3(m.clearcoatGloss, m.IOR, m.transmission);

            m_Triangles.push_back(t);
            m_TriangleEncodeds.push_back(te);

        }
    }
    else
    {
        printf("Unable to load model %s\n", filename.c_str());
        delete model;
        id = -1;
    }
    //std::cout << "Triangles : " << m_Triangles.size() << std::endl;
    //std::cout << "Triangle encode : " << m_TriangleEncodeds.size() << std::endl;

    return id;
}


// 构建 BVH
int Scene::buildBVH(int l, int r) {
    if (l > r) return 0;

    // 注：
    // 此处不可通过指针，引用等方式操作，必须用 m_BVHNodes[id] 来操作
    // 因为 std::vector<> 扩容时会拷贝到更大的内存，那么地址就改变了
    // 而指针，引用均指向原来的内存，所以会发生错误
    m_BVHNodes.push_back(BVHNode());
    int id = m_BVHNodes.size() - 1;   // 注意： 先保存索引 id = 0;
    m_BVHNodes[id].left = m_BVHNodes[id].right = m_BVHNodes[id].n = m_BVHNodes[id].index = 0;
    m_BVHNodes[id].AA = glm::vec3(1145141919, 1145141919, 1145141919);
    m_BVHNodes[id].BB = glm::vec3(-1145141919, -1145141919, -1145141919);

    // 计算 AABB
    for (int i = l; i <= r; i++) {
        // 最小点 AA
        float minx = std::min(m_Triangles[i].position[0].x, std::min(m_Triangles[i].position[1].x, m_Triangles[i].position[2].x));
        float miny = std::min(m_Triangles[i].position[0].y, std::min(m_Triangles[i].position[1].y, m_Triangles[i].position[2].y));
        float minz = std::min(m_Triangles[i].position[0].z, std::min(m_Triangles[i].position[1].z, m_Triangles[i].position[2].z));
        
        m_BVHNodes[id].AA.x = std::min(m_BVHNodes[id].AA.x, minx);
        m_BVHNodes[id].AA.y = std::min(m_BVHNodes[id].AA.y, miny);
        m_BVHNodes[id].AA.z = std::min(m_BVHNodes[id].AA.z, minz);
        // 最大点 BB
        float maxx = std::max(m_Triangles[i].position[0].x, std::max(m_Triangles[i].position[1].x, m_Triangles[i].position[2].x));
        float maxy = std::max(m_Triangles[i].position[0].y, std::max(m_Triangles[i].position[1].y, m_Triangles[i].position[2].y));
        float maxz = std::max(m_Triangles[i].position[0].z, std::max(m_Triangles[i].position[1].z, m_Triangles[i].position[2].z));
        m_BVHNodes[id].BB.x = std::max(m_BVHNodes[id].BB.x, maxx);
        m_BVHNodes[id].BB.y = std::max(m_BVHNodes[id].BB.y, maxy);
        m_BVHNodes[id].BB.z = std::max(m_BVHNodes[id].BB.z, maxz);
    }

    // 不多于 n 个三角形 返回叶子节点
    if ((r - l + 1) <= leafNum) {
        m_BVHNodes[id].n = r - l + 1;
        m_BVHNodes[id].index = l;
        return id;
    }

    // 否则递归建树
    float lenx = m_BVHNodes[id].BB.x - m_BVHNodes[id].AA.x;
    float leny = m_BVHNodes[id].BB.y - m_BVHNodes[id].AA.y;
    float lenz = m_BVHNodes[id].BB.z - m_BVHNodes[id].AA.z;
    // 按 x 划分
    if (lenx >= leny && lenx >= lenz)
        std::sort(m_Triangles.begin() + l, m_Triangles.begin() + r + 1, cmpx);
    // 按 y 划分
    if (leny >= lenx && leny >= lenz)
        std::sort(m_Triangles.begin() + l, m_Triangles.begin() + r + 1, cmpy);
    // 按 z 划分
    if (lenz >= lenx && lenz >= leny)
        std::sort(m_Triangles.begin() + l, m_Triangles.begin() + r + 1, cmpz);
    // 递归
    int mid = (l + r) / 2;
    int left = buildBVH(l, mid);
    int right = buildBVH(mid + 1, r);

    m_BVHNodes[id].left = left;
    m_BVHNodes[id].right = right;

    return id;
}

// SAH 优化构建 BVH
int Scene::buildBVHwithSAH(int l, int r) {
    if (l > r) return 0;

    m_BVHNodes.push_back(BVHNode());
    int id = m_BVHNodes.size() - 1;
    m_BVHNodes[id].left = m_BVHNodes[id].right = m_BVHNodes[id].n = m_BVHNodes[id].index = 0;
    m_BVHNodes[id].AA = glm::vec3(1145141919, 1145141919, 1145141919);
    m_BVHNodes[id].BB = glm::vec3(-1145141919, -1145141919, -1145141919);

    // 计算 AABB
    for (int i = l; i <= r; i++) {
        float minx = std::min(m_Triangles[i].position[0].x, std::min(m_Triangles[i].position[1].x, m_Triangles[i].position[2].x));
        float miny = std::min(m_Triangles[i].position[0].y, std::min(m_Triangles[i].position[1].y, m_Triangles[i].position[2].y));
        float minz = std::min(m_Triangles[i].position[0].z, std::min(m_Triangles[i].position[1].z, m_Triangles[i].position[2].z));

        m_BVHNodes[id].AA.x = std::min(m_BVHNodes[id].AA.x, minx);
        m_BVHNodes[id].AA.y = std::min(m_BVHNodes[id].AA.y, miny);
        m_BVHNodes[id].AA.z = std::min(m_BVHNodes[id].AA.z, minz);
        // 最大点 BB
        float maxx = std::max(m_Triangles[i].position[0].x, std::max(m_Triangles[i].position[1].x, m_Triangles[i].position[2].x));
        float maxy = std::max(m_Triangles[i].position[0].y, std::max(m_Triangles[i].position[1].y, m_Triangles[i].position[2].y));
        float maxz = std::max(m_Triangles[i].position[0].z, std::max(m_Triangles[i].position[1].z, m_Triangles[i].position[2].z));
        m_BVHNodes[id].BB.x = std::max(m_BVHNodes[id].BB.x, maxx);
        m_BVHNodes[id].BB.y = std::max(m_BVHNodes[id].BB.y, maxy);
        m_BVHNodes[id].BB.z = std::max(m_BVHNodes[id].BB.z, maxz);
    }

    // 不多于 n 个三角形 返回叶子节点
    if ((r - l + 1) <= leafNum) {
        m_BVHNodes[id].n = r - l + 1;
        m_BVHNodes[id].index = l;
        return id;
    }

    // 否则递归建树
    float Cost = INF;
    int Axis = 0;
    int Split = (l + r) / 2;
    for (int axis = 0; axis < 3; axis++) {
        // 分别按 x，y，z 轴排序
        if (axis == 0) std::sort(&m_Triangles[0] + l, &m_Triangles[0] + r + 1, cmpx);
        if (axis == 1) std::sort(&m_Triangles[0] + l, &m_Triangles[0] + r + 1, cmpy);
        if (axis == 2) std::sort(&m_Triangles[0] + l, &m_Triangles[0] + r + 1, cmpz);

        // leftMax[i]: [l, i] 中最大的 xyz 值
        // leftMin[i]: [l, i] 中最小的 xyz 值
        std::vector<glm::vec3> leftMax(r - l + 1, glm::vec3(-INF, -INF, -INF));
        std::vector<glm::vec3> leftMin(r - l + 1, glm::vec3(INF, INF, INF));
        // 计算前缀 注意 i-l 以对齐到下标 0
        for (int i = l; i <= r; i++) {
            Triangle& t = m_Triangles[i];
            int bias = (i == l) ? 0 : 1;  // 第一个元素特殊处理

            leftMax[i - l].x = std::max(leftMax[i - l + bias].x, std::max(t.position[0].x, std::max(t.position[1].x, t.position[2].x)));
            leftMax[i - l].y = std::max(leftMax[i - l + bias].y, std::max(t.position[0].y, std::max(t.position[1].y, t.position[2].y)));
            leftMax[i - l].z = std::max(leftMax[i - l + bias].z, std::max(t.position[0].z, std::max(t.position[1].z, t.position[2].z)));
            leftMin[i - l].x = std::min(leftMin[i - l + bias].x, std::min(t.position[0].x, std::min(t.position[1].x, t.position[2].x)));
            leftMin[i - l].y = std::min(leftMin[i - l + bias].y, std::min(t.position[0].y, std::min(t.position[1].y, t.position[2].y)));
            leftMin[i - l].z = std::min(leftMin[i - l + bias].z, std::min(t.position[0].z, std::min(t.position[1].z, t.position[2].z)));
        }

        // rightMax[i]: [i, r] 中最大的 xyz 值
        // rightMin[i]: [i, r] 中最小的 xyz 值
        std::vector<glm::vec3> rightMax(r - l + 1, glm::vec3(-INF, -INF, -INF));
        std::vector<glm::vec3> rightMin(r - l + 1, glm::vec3(INF, INF, INF));
        // 计算后缀 注意 i-l 以对齐到下标 0
        for (int i = r; i >= l; i--) {
            Triangle& t = m_Triangles[i];
            int bias = (i == r) ? 0 : 1;  // 第一个元素特殊处理

            rightMax[i - l].x = std::max(rightMax[i - l + bias].x, std::max(t.position[0].x, std::max(t.position[1].x, t.position[2].x)));
            rightMax[i - l].y = std::max(rightMax[i - l + bias].y, std::max(t.position[0].y, std::max(t.position[1].y, t.position[2].y)));
            rightMax[i - l].z = std::max(rightMax[i - l + bias].z, std::max(t.position[0].z, std::max(t.position[1].z, t.position[2].z)));

            rightMin[i - l].x = std::min(rightMin[i - l + bias].x, std::min(t.position[0].x, std::min(t.position[1].x, t.position[2].x)));
            rightMin[i - l].y = std::min(rightMin[i - l + bias].y, std::min(t.position[0].y, std::min(t.position[1].y, t.position[2].y)));
            rightMin[i - l].z = std::min(rightMin[i - l + bias].z, std::min(t.position[0].z, std::min(t.position[1].z, t.position[2].z)));
        }

        // 遍历寻找分割
        float cost = INF;
        int split = l;
        for (int i = l; i <= r - 1; i++) {
            float lenx, leny, lenz;
            // 左侧 [l, i]
            glm::vec3 leftAA = leftMin[i - l];
            glm::vec3 leftBB = leftMax[i - l];
            lenx = leftBB.x - leftAA.x;
            leny = leftBB.y - leftAA.y;
            lenz = leftBB.z - leftAA.z;
            float leftS = 2.0 * ((lenx * leny) + (lenx * lenz) + (leny * lenz));
            float leftCost = leftS * (i - l + 1);

            // 右侧 [i+1, r]
            glm::vec3 rightAA = rightMin[i + 1 - l];
            glm::vec3 rightBB = rightMax[i + 1 - l];
            lenx = rightBB.x - rightAA.x;
            leny = rightBB.y - rightAA.y;
            lenz = rightBB.z - rightAA.z;
            float rightS = 2.0 * ((lenx * leny) + (lenx * lenz) + (leny * lenz));
            float rightCost = rightS * (r - i);

            // 记录每个分割的最小答案
            float totalCost = leftCost + rightCost;
            if (totalCost < cost) {
                cost = totalCost;
                split = i;
            }
        }
        // 记录每个轴的最佳答案
        if (cost < Cost) {
            Cost = cost;
            Axis = axis;
            Split = split;
        }
    }

    // 按最佳轴分割
    if (Axis == 0) std::sort(&m_Triangles[0] + l, &m_Triangles[0] + r + 1, cmpx);
    if (Axis == 1) std::sort(&m_Triangles[0] + l, &m_Triangles[0] + r + 1, cmpy);
    if (Axis == 2) std::sort(&m_Triangles[0] + l, &m_Triangles[0] + r + 1, cmpz);

    // 递归
    int left = buildBVHwithSAH(l, Split);
    int right = buildBVHwithSAH(Split + 1, r);

    m_BVHNodes[id].left = left;
    m_BVHNodes[id].right = right;

    return id;
}

void Scene::translateTriangleEncode()
{
    int nNodes = m_BVHNodes.size();
    m_BVHNodeEncodeds.resize(nNodes);

    for (int i = 0; i < m_BVHNodes.size(); i++)
    {
        m_BVHNodeEncodeds[i].childs   = glm::vec3(m_BVHNodes[i].left, m_BVHNodes[i].right, 0);
        m_BVHNodeEncodeds[i].leafInfo = glm::vec3(m_BVHNodes[i].n, m_BVHNodes[i].index, 0);
        m_BVHNodeEncodeds[i].AA = m_BVHNodes[i].AA;
        m_BVHNodeEncodeds[i].BB = m_BVHNodes[i].BB;
    }
    //std::cout << "address is " << &m_BVHNodeEncodeds[0] << "\n";
}

