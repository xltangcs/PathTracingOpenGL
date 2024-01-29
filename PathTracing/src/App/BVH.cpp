//#include "BVH.h"
//
//#include <algorithm>
//#include <iostream>
//
//const float INF = 0x3f3f3f3f;
//
//// ----------------------------------------------------------------------------- //
//
////按照三角形中心排序 -- 比较函数
//bool cmpx(const Triangle& t1, const Triangle& t2) {
//    glm::vec3 center1 = (t1.position[0] + t1.position[1] + t1.position[2]) / glm::vec3(3, 3, 3);
//    glm::vec3 center2 = (t2.position[0] + t2.position[1] + t2.position[2]) / glm::vec3(3, 3, 3);
//    return center1.x < center2.x;
//}
//bool cmpy(const Triangle& t1, const Triangle& t2) {
//    glm::vec3 center1 = (t1.position[0] + t1.position[1] + t1.position[2]) / glm::vec3(3, 3, 3);
//    glm::vec3 center2 = (t2.position[0] + t2.position[1] + t2.position[2]) / glm::vec3(3, 3, 3);
//    return center1.y < center2.y;
//}
//bool cmpz(const Triangle& t1, const Triangle& t2) {
//    glm::vec3 center1 = (t1.position[0] + t1.position[1] + t1.position[2]) / glm::vec3(3, 3, 3);
//    glm::vec3 center2 = (t2.position[0] + t2.position[1] + t2.position[2]) / glm::vec3(3, 3, 3);
//    return center1.z < center2.z;
//}
//
//BVHNode* BVH::buildBVH(std::vector<Triangle>& triangles, int l, int r)
//{
//	if (l > r) return nullptr;
//
//	BVHNode* node = new BVHNode();
//
//    node->SerialNum = serialNum++;
//	node->AA = glm::vec3(INF, INF, INF); //min bound
//	node->BB = glm::vec3(-INF, -INF, -INF); //max bound
//
//    // 计算 区间 [l, r] AABB
//    for (int i = l; i <= r; i++) {
//        // 最小点 AA
//        float minx = std::min(triangles[i].position[0].x, std::min(triangles[i].position[1].x, triangles[i].position[2].x));
//        float miny = std::min(triangles[i].position[0].y, std::min(triangles[i].position[1].y, triangles[i].position[2].y));
//        float minz = std::min(triangles[i].position[0].z, std::min(triangles[i].position[1].z, triangles[i].position[2].z));
//
//       node->AA.x = std::min(node->AA.x, minx);
//       node->AA.y = std::min(node->AA.y, miny);
//       node->AA.z = std::min(node->AA.z, minz);
//        // 最大点 BB
//        float maxx = std::max(triangles[i].position[0].x, std::max(triangles[i].position[1].x, triangles[i].position[2].x));
//        float maxy = std::max(triangles[i].position[0].y, std::max(triangles[i].position[1].y, triangles[i].position[2].y));
//        float maxz = std::max(triangles[i].position[0].z, std::max(triangles[i].position[1].z, triangles[i].position[2].z));
//        node->BB.x = std::max(node->BB.x, maxx);
//        node->BB.y = std::max(node->BB.y, maxy);
//        node->BB.z = std::max(node->BB.z, maxz);
//    }
//
//    if (r - l + 1 <= leafNum) //[3, 7] = 5
//    {
//        node->n = r - l + 1;
//        node->index = l;
//        node->isLeaf = 1;
//        return node;
//    }
//
//    // 否则递归建树
//    float lenx = node->BB.x - node->AA.x;
//    float leny = node->BB.y - node->AA.y;
//    float lenz = node->BB.z - node->AA.z;
//    // 按 x 划分
//    if (lenx >= leny && lenx >= lenz)
//        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpx);
//    // 按 y 划分
//    if (leny >= lenx && leny >= lenz)
//        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpy);
//    // 按 z 划分
//    if (lenz >= lenx && lenz >= leny)
//        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpz);
//    // 递归
//    int mid = (l + r) / 2;
//    node->left = buildBVH(triangles, l, mid);
//    node->right = buildBVH(triangles, mid + 1, r);
//    node->n = r - l + 1;
//    node->index = l;
//    node->isLeaf = 0;
//    return node;
//}
//
//void BVH::flattenBVHTree(BVHNode* node)
//{
//    if (node == nullptr) return;
//    
//    int id = m_BVHNodeEncodeeds.size();
//
//    BVHNodeEncoded nodeEncodeed;
//
//    nodeEncodeed.AA = node->AA;
//    nodeEncodeed.BB = node->BB;
//    nodeEncodeed.leafInfo = glm::vec3(node->n, node->index, node->isLeaf);
//
//    nodeEncodeed.childs = glm::vec3(node->left == nullptr ? -1 : node->left->SerialNum,
//        node->right == nullptr ? -1 : node->right->SerialNum,
//        0);
//
//    m_BVHNodeEncodeeds.push_back(nodeEncodeed);
//
//    flattenBVHTree(node->left);
//    flattenBVHTree(node->right);
//
//}
//
//void BVH::printBVHNodeEncodeed()
//{
//    int n = m_BVHNodeEncodeeds.size();
//    std::cout << "BVH Node num is : " << n << std::endl;
//    for (size_t i = 0; i < n; i++)
//    {
//        std::cout << "i : " << i;
//        std::cout << "\tleft = " << m_BVHNodeEncodeeds[i].childs.x
//            << "\tright = " << m_BVHNodeEncodeeds[i].childs.y;
//
//        std::cout << "\tn = " << m_BVHNodeEncodeeds[i].leafInfo.x
//            << "\tindex = " << m_BVHNodeEncodeeds[i].leafInfo.y;
//
//
//        std::string leaf = m_BVHNodeEncodeeds[i].leafInfo.z ? "\tIs Leaf" : "\tNo Leaf";
//        std::cout << leaf << "\n";
//    }
//}
