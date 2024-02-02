#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "App/Triangle.h"

struct BVHNode
{
	//BVHNode* left = nullptr;
	//BVHNode* right = nullptr;
	int left = -1, right = -1;
	int n = 0;
	int index = 0;
	int isLeaf = 0;
	glm::vec3 AA, BB;
};

struct BVHNodeEncoded {
	glm::vec3 childs;   // (left, right, ±£¡Ù)
	glm::vec3 leafInfo; // (n, index, isLeaf)
	glm::vec3 AA, BB;
};



class BVH
{
public:
	BVH()= default;
	~BVH()= default;

	int buildBVH(std::vector<TriangleEncoded>& triangles, int left, int right);

	int buildBVHwithSAH(std::vector<TriangleEncoded>& triangles, int left, int right);


	void flattenBVHTree(BVHNode* node);

	void translateBVHNodes(std::vector<BVHNodeEncoded>& m_BVHNodeEncodeeds);

	void printBVHNodeEncodeed(std::vector<BVHNodeEncoded>& bVHNodeEncodeeds);

	const int GetBVHNodeNum() const { return m_BVHNodeEncodeeds.size(); }
	const void* GetBVHNodeEncodedAddr() const { return &m_BVHNodeEncodeeds[0]; }

private:
	

private:
	int leafNum = 8;
	int serialNum = 0;

	std::vector<BVHNodeEncoded> m_BVHNodeEncodeeds;
	std::vector<BVHNode> m_BVHNodes;
};