#version 330 core

out vec4 fragColor;
in vec3 pix;

uniform int frameCounter;

uniform int nTriangles;
uniform int nNodes;

uniform int width;
uniform int height;

uniform samplerBuffer TrianglesTexture;
uniform samplerBuffer BVHNodesTexture;

uniform vec3 cameraPosition;
uniform vec3 cameraRotate;

uniform int setBVH;


// ----------------------------------------------------------------------------- //

#define PI              3.1415926
#define INF             114514.0
#define SIZE_TRIANGLE   12
#define SIZE_BVHNODE    4

// ----------------------------------------------------------------------------- //

// Triangle 数据格式
struct Triangle {
    vec3 p1, p2, p3;    // 顶点坐标
    vec3 n1, n2, n3;    // 顶点法线
};

// BVH 树节点
struct BVHNode {
    int left;           // 左子树
    int right;          // 右子树
    int n;              // 包含三角形数目
    int index;          // 三角形索引
    bool isLeaf;
    vec3 AA, BB;        // 碰撞盒
};

// 光线
struct Ray {
    vec3 startPoint;
    vec3 direction;
};

// 物体表面材质定义
struct Material {
    vec3 emissive;          // 作为光源时的发光颜色
    vec3 baseColor;
    float subsurface;
    float metallic;
    float specular;
    float specularTint;
    float roughness;
    float anisotropic;
    float sheen;
    float sheenTint;
    float clearcoat;
    float clearcoatGloss;
    float IOR;
    float transmission;
};

// 光线求交结果
struct HitResult {
    bool isHit;             // 是否命中
    bool isInside;          // 是否从内部命中
    float distance;         // 与交点的距离
    vec3 hitPoint;          // 光线命中点
    vec3 normal;            // 命中点法线
    vec3 viewDir;           // 击中该点的光线的方向
    Material material;      // 命中点的表面材质
};

// ----------------------------------------------------------------------------- //

// 获取第 i 下标的三角形
Triangle getTriangle(int i) {
    int offset = i * SIZE_TRIANGLE;
    Triangle t;

    // 顶点坐标
    t.p1 = texelFetch(TrianglesTexture, offset + 0).xyz;
    t.p2 = texelFetch(TrianglesTexture, offset + 1).xyz;
    t.p3 = texelFetch(TrianglesTexture, offset + 2).xyz;
    // 法线           
    t.n1 = texelFetch(TrianglesTexture, offset + 3).xyz;
    t.n2 = texelFetch(TrianglesTexture, offset + 4).xyz;
    t.n3 = texelFetch(TrianglesTexture, offset + 5).xyz;

    return t;
}
// 获取第 i 下标的三角形的材质
Material getMaterial(int i) {
    Material m;

    int offset = i * SIZE_TRIANGLE;
    vec3 param1 = texelFetch(TrianglesTexture, offset + 8).xyz;
    vec3 param2 = texelFetch(TrianglesTexture, offset + 9).xyz;
    vec3 param3 = texelFetch(TrianglesTexture, offset + 10).xyz;
    vec3 param4 = texelFetch(TrianglesTexture, offset + 11).xyz;
    
    m.emissive = texelFetch(TrianglesTexture, offset + 6).xyz;
    m.baseColor = texelFetch(TrianglesTexture, offset + 7).xyz;
    m.subsurface = param1.x;
    m.metallic = param1.y;
    m.specular = param1.z;
    m.specularTint = param2.x;
    m.roughness = param2.y;
    m.anisotropic = param2.z;
    m.sheen = param3.x;
    m.sheenTint = param3.y;
    m.clearcoat = param3.z;
    m.clearcoatGloss = param4.x;
    m.IOR = param4.y;
    m.transmission = param4.z;

    return m;
}

// 获取第 i 下标的 BVHNode 对象
BVHNode getBVHNode(int i) {
    BVHNode node;

    // 左右子树
    int offset = i * SIZE_BVHNODE;
    ivec3 childs = ivec3(texelFetch(BVHNodesTexture, offset + 0).xyz);
    ivec3 leafInfo = ivec3(texelFetch(BVHNodesTexture, offset + 1).xyz);
    node.left = int(childs.x);
    node.right = int(childs.y);
    node.n = int(leafInfo.x);
    node.index = int(leafInfo.y);
    node.isLeaf = int(leafInfo.z) == 1;

    // 包围盒
    node.AA = texelFetch(BVHNodesTexture, offset + 2).xyz;
    node.BB = texelFetch(BVHNodesTexture, offset + 3).xyz;

    return node;
}
// ----------------------------------------------------------------------------- //

/*
 * 生成随机向量，依赖于 frameCounter 帧计数器
 * 代码来源：https://blog.demofox.org/2020/05/25/casual-shadertoy-path-tracing-1-basic-camera-diffuse-emissive/
*/

uint seed = uint(
    uint((pix.x * 0.5 + 0.5) * width)  * uint(1973) + 
    uint((pix.y * 0.5 + 0.5) * height) * uint(9277) + 
    uint(frameCounter) * uint(26699)) | uint(1);

uint wang_hash(inout uint seed) {
    seed = uint(seed ^ uint(61)) ^ uint(seed >> uint(16));
    seed *= uint(9);
    seed = seed ^ (seed >> 4);
    seed *= uint(0x27d4eb2d);
    seed = seed ^ (seed >> 15);
    return seed;
}
 
float rand() {
    return float(wang_hash(seed)) / 4294967296.0;
}


// ----------------------------------------------------------------------------- //

// 光线和三角形求交 
HitResult hitTriangle(Triangle triangle, Ray ray) {
    HitResult res;
    res.distance = INF;
    res.isHit = false;
    res.isInside = false;

    vec3 p1 = triangle.p1;
    vec3 p2 = triangle.p2;
    vec3 p3 = triangle.p3;

    vec3 S = ray.startPoint;    // 射线起点
    vec3 d = ray.direction;     // 射线方向
    vec3 N = normalize(cross(p2-p1, p3-p1));    // 法向量

    // 从三角形背后（模型内部）击中
    if (dot(N, d) > 0.0f) {
        N = -N;   
        res.isInside = true;
    }

    // 如果视线和三角形平行
    if (abs(dot(N, d)) < 0.00001f) return res;

    // 距离
    float t = (dot(N, p1) - dot(S, N)) / dot(d, N);
    if (t < 0.0005f) return res;    // 如果三角形在光线背面

    // 交点计算
    vec3 P = S + d * t;

    // 判断交点是否在三角形中
    vec3 c1 = cross(p2 - p1, P - p1);
    vec3 c2 = cross(p3 - p2, P - p2);
    vec3 c3 = cross(p1 - p3, P - p3);
    bool r1 = (dot(c1, N) > 0 && dot(c2, N) > 0 && dot(c3, N) > 0);
    bool r2 = (dot(c1, N) < 0 && dot(c2, N) < 0 && dot(c3, N) < 0);

    // 命中，封装返回结果
    if (r1 || r2) {
        res.isHit = true;
        res.hitPoint = P;
        res.distance = t;
        res.normal = N;
        res.viewDir = d;
        // 根据交点位置插值顶点法线
        float alpha = (-(P.x-p2.x)*(p3.y-p2.y) + (P.y-p2.y)*(p3.x-p2.x)) / (-(p1.x-p2.x-0.00005)*(p3.y-p2.y+0.00005) + (p1.y-p2.y+0.00005)*(p3.x-p2.x+0.00005));
        float beta  = (-(P.x-p3.x)*(p1.y-p3.y) + (P.y-p3.y)*(p1.x-p3.x)) / (-(p2.x-p3.x-0.00005)*(p1.y-p3.y+0.00005) + (p2.y-p3.y+0.00005)*(p1.x-p3.x+0.00005));
        float gama  = 1.0 - alpha - beta;
        vec3 Nsmooth = alpha * triangle.n1 + beta * triangle.n2 + gama * triangle.n3;
        Nsmooth = normalize(Nsmooth);
        res.normal = (res.isInside) ? (-Nsmooth) : (Nsmooth);
    }

    return res;
}

// 和 aabb 盒子求交，没有交点则返回 -1
float hitAABB(Ray r, vec3 AA, vec3 BB) {
    vec3 invdir = 1.0 / r.direction;

    vec3 f = (BB - r.startPoint) * invdir;
    vec3 n = (AA - r.startPoint) * invdir;

    vec3 tmax = max(f, n);
    vec3 tmin = min(f, n);

    float t1 = min(tmax.x, min(tmax.y, tmax.z));
    float t0 = max(tmin.x, max(tmin.y, tmin.z));

    return (t1 >= t0) ? ((t0 > 0.0) ? (t0) : (t1)) : (-1);
}

// ----------------------------------------------------------------------------- //

// 暴力遍历数组下标范围 [l, r] 求最近交点
HitResult hitArray(Ray ray, int l, int r) {
    HitResult res;
    res.isHit = false;
    res.distance = INF;
    for(int i=l; i<=r; i++) {
        Triangle triangle = getTriangle(i);
        HitResult hitRes = hitTriangle(triangle, ray);
        if(hitRes.isHit && hitRes.distance<res.distance) {
            res = hitRes;
            res.material = getMaterial(i);
        }
    }
    return res;
}


// 遍历 BVH 求交
HitResult hitBVH(Ray ray) {
    HitResult res;
    res.isHit = false;
    res.distance = INF;

    // 栈
    int stack[256];
    int sp = 0;

    stack[sp++] = 1;
    while(sp>0) {
        int top = stack[--sp];
        BVHNode node = getBVHNode(top);
        
        // 是叶子节点，遍历三角形，求最近交点
        if(node.n>0) {
            int L = node.index;
            int R = node.index + node.n - 1;
            HitResult r = hitArray(ray, L, R);
            if(r.isHit && r.distance<res.distance) res = r;
            //continue;
            break;
        }
        
        // 和左右盒子 AABB 求交
        float d1 = INF; // 左盒子距离
        float d2 = INF; // 右盒子距离
        if(node.left>0) {
            BVHNode leftNode = getBVHNode(node.left);
            d1 = hitAABB(ray, leftNode.AA, leftNode.BB);
        }
        if(node.right>0) {
            BVHNode rightNode = getBVHNode(node.right);
            d2 = hitAABB(ray, rightNode.AA, rightNode.BB);
        }

        // 在最近的盒子中搜索
        if(d1>0 && d2>0) {
            if(d1<d2) { // d1<d2, 左边先
                stack[sp++] = node.right;
                stack[sp++] = node.left;
            } else {    // d2<d1, 右边先
                stack[sp++] = node.left;
                stack[sp++] = node.right;
            }
        } else if(d1>0) {   // 仅命中左边
            stack[sp++] = node.left;
        } else if(d2>0) {   // 仅命中右边
            stack[sp++] = node.right;
        }
    }

    return res;
}

void main()
{
    // 投射光线
    Ray ray;
    ray.startPoint = cameraPosition;
    vec3 dir = vec3(pix.xy, -1) - ray.startPoint;
    ray.direction = normalize(dir);

    //fragColor = vec4(nNodes == 15, 0, 0, 0);

    //for(int i=0; i<nNodes; i++)
    //{
    //    BVHNode node = getBVHNode(i);
    //    //if(node.left == 6 && node.right ==8 && i==9)
    //    if(node.n == 3 && node.index == 6 && node.isLeaf == false)
    //        fragColor = vec4(1, 0, 0, 0);
    //}

    //if(node.isLeaf)
    //{
    //    Triangle triangle = getTriangle(node.index);
    //    HitResult res = hitArray(ray, node.index, node.index);
    //    fragColor = vec4(res.material.baseColor, 0);
    //}


    //for(int i=0; i<nNodes; i++)
    //{
    //    BVHNode node = getBVHNode(i);
    //    if(node.isLeaf)  //leaf
    //    {
    //        int L = node.index;
    //        int R = node.index + node.n - 1;
    //        HitResult res = hitArray(ray, L, R);
    //        if(res.isHit){
    //            fragColor = vec4(res.material.baseColor, 1);
    //        }
    //    }
    //}  

    //BVHNode node = getBVHNode(0);
    //BVHNode left = getBVHNode(node.left);
    //BVHNode right = getBVHNode(node.right);
    //float r1 = hitAABB(ray, left.AA, left.BB);  
    //float r2 = hitAABB(ray, right.AA, right.BB); 
    //
    //vec3 color;
    //if(r1>0) color = vec3(1, 0, 0);
    //if(r2>0) color = vec3(0, 1, 0);
    //if(r1>0 && r2>0) color = vec3(1, 1, 0);
    //fragColor = vec4(color, 1);



    HitResult res;
    if(setBVH == 0)
    {
        res = hitArray(ray, 0, nTriangles-1);
        fragColor = vec4(res.material.baseColor.x, 0, 0, 1);
    }
    //res = hitArray(ray, 0, nTriangles-1);
    else
    {
        res = hitBVH(ray);
        fragColor = vec4(0, res.material.baseColor.y, 0, 1);
    }
    //res = hitBVH(ray);

    //if(res.isHit) fragColor = vec4(res.material.baseColor, 1);
    //else 
    //fragColor = vec4(0.2, 0.2, 0.3, 1);
}