#include "BVH.h"

#include <algorithm>
#include <iostream>

const float INF = 0x3f3f3f3f;

// ----------------------------------------------------------------------------- //

//按照三角形中心排序 -- 比较函数
bool cmpx(const TriangleEncoded& t1, const TriangleEncoded& t2) {
    glm::vec3 center1 = (t1.p1 + t1.p2 + t1.p3) / glm::vec3(3, 3, 3);
    glm::vec3 center2 = (t2.p1 + t2.p2 + t2.p3) / glm::vec3(3, 3, 3);
    return center1.x < center2.x;
}
bool cmpy(const TriangleEncoded& t1, const TriangleEncoded& t2) {
    glm::vec3 center1 = (t1.p1 + t1.p2 + t1.p3) / glm::vec3(3, 3, 3);
    glm::vec3 center2 = (t2.p1 + t2.p2 + t2.p3) / glm::vec3(3, 3, 3);
    return center1.y < center2.y;
}
bool cmpz(const TriangleEncoded& t1, const TriangleEncoded& t2) {
    glm::vec3 center1 = (t1.p1 + t1.p2 + t1.p3) / glm::vec3(3, 3, 3);
    glm::vec3 center2 = (t2.p1 + t2.p2 + t2.p3) / glm::vec3(3, 3, 3);
    return center1.z < center2.z;
}

int BVH::buildBVH(std::vector<TriangleEncoded>& triangles, int l, int r)
{
	if (l > r) return -1;

	BVHNode node;
    m_BVHNodes.push_back(node);
    int id = m_BVHNodes.size() - 1;

	m_BVHNodes[id].AA = glm::vec3(INF, INF, INF); //min bound
	m_BVHNodes[id].BB = glm::vec3(-INF, -INF, -INF); //max bound

    // 计算 区间 [l, r] AABB
    for (int i = l; i <= r; i++) {
        // 最小点 AA
        float minx = std::min(triangles[i].p1.x, std::min(triangles[i].p2.x, triangles[i].p3.x));
        float miny = std::min(triangles[i].p1.y, std::min(triangles[i].p2.y, triangles[i].p3.y));
        float minz = std::min(triangles[i].p1.z, std::min(triangles[i].p2.z, triangles[i].p3.z));
        m_BVHNodes[id].AA.x = std::min(m_BVHNodes[id].AA.x, minx);
        m_BVHNodes[id].AA.y = std::min(m_BVHNodes[id].AA.y, miny);
        m_BVHNodes[id].AA.z = std::min(m_BVHNodes[id].AA.z, minz);
        // 最大点 BB
        float maxx = std::max(triangles[i].p1.x, std::max(triangles[i].p2.x, triangles[i].p3.x));
        float maxy = std::max(triangles[i].p1.y, std::max(triangles[i].p2.y, triangles[i].p3.y));
        float maxz = std::max(triangles[i].p1.z, std::max(triangles[i].p2.z, triangles[i].p3.z));
        m_BVHNodes[id].BB.x = std::max(m_BVHNodes[id].BB.x, maxx);
        m_BVHNodes[id].BB.y = std::max(m_BVHNodes[id].BB.y, maxy);
        m_BVHNodes[id].BB.z = std::max(m_BVHNodes[id].BB.z, maxz);
    }

    if (r - l + 1 <= leafNum) //[3, 7] = 5
    {
        m_BVHNodes[id].n = r - l + 1;
        m_BVHNodes[id].index = l;
        m_BVHNodes[id].isLeaf = 1;
        return id;
    }

    // 否则递归建树
    float lenx = m_BVHNodes[id].BB.x - m_BVHNodes[id].AA.x;
    float leny = m_BVHNodes[id].BB.y - m_BVHNodes[id].AA.y;
    float lenz = m_BVHNodes[id].BB.z - m_BVHNodes[id].AA.z;
    // 按 x 划分
    if (lenx >= leny && lenx >= lenz)
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpx);
    // 按 y 划分
    if (leny >= lenx && leny >= lenz)
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpy);
    // 按 z 划分
    if (lenz >= lenx && lenz >= leny)
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpz);
    // 递归
    int mid = (l + r) / 2;
    m_BVHNodes[id].left = buildBVH(triangles, l, mid);
    m_BVHNodes[id].right = buildBVH(triangles, mid + 1, r);
    m_BVHNodes[id].n = r - l + 1;
    m_BVHNodes[id].index = l;

    return id;
}

void BVH::flattenBVHTree(BVHNode* node)
{
    //if (node == nullptr) return;
    //
    //int id = m_BVHNodeEncodeeds.size();

    //BVHNodeEncoded nodeEncodeed;

    //nodeEncodeed.AA = node->AA;
    //nodeEncodeed.BB = node->BB;
    //nodeEncodeed.leafInfo = glm::vec3(node->n, node->index, node->isLeaf);

    //nodeEncodeed.childs = glm::vec3(node->left == nullptr ? -1 : node->left->SerialNum,
    //    node->right == nullptr ? -1 : node->right->SerialNum,
    //    0);

    //m_BVHNodeEncodeeds.push_back(nodeEncodeed);

    //flattenBVHTree(node->left);
    //flattenBVHTree(node->right);

}

void BVH::printBVHNodeEncodeed(std::vector<BVHNodeEncoded>& bvhNodeEncodeeds)
{
    int n = bvhNodeEncodeeds.size();
    std::cout << "BVH Node num is : " << n << std::endl;
    for (size_t i = 0; i < n; i++)
    {
        
        if (1)
        {
            std::cout << "i : " << i;
            std::cout << "\tleft = " << bvhNodeEncodeeds[i].childs.x
                << "\tright = " << bvhNodeEncodeeds[i].childs.y;

            std::cout << "\tn = " << bvhNodeEncodeeds[i].leafInfo.x
                << "\tindex = " << bvhNodeEncodeeds[i].leafInfo.y;


            std::string leaf = bvhNodeEncodeeds[i].leafInfo.z ? "\tIs Leaf" : "\tNo Leaf";
            std::cout << leaf << "\n";
        }
    }
}

void BVH::translateBVHNodes(std::vector<BVHNodeEncoded>& bvhNodeEncodeeds)
{
    int nNodes = m_BVHNodes.size();
    bvhNodeEncodeeds.resize(nNodes);
    for (int i = 0; i < nNodes; i++)
    {
        bvhNodeEncodeeds[i].childs = glm::vec3(m_BVHNodes[i].left, m_BVHNodes[i].right, 0);
        bvhNodeEncodeeds[i].leafInfo = glm::vec3(m_BVHNodes[i].n, m_BVHNodes[i].index, m_BVHNodes[i].isLeaf);
        bvhNodeEncodeeds[i].AA = m_BVHNodes[i].AA;
        bvhNodeEncodeeds[i].BB = m_BVHNodes[i].BB;
    }
}
