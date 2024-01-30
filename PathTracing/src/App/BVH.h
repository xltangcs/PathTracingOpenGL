#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "App/Triangle.h"

struct BVHNode
{
	BVHNode* left = nullptr;
	BVHNode* right = nullptr;
	int n = -1;
	int index = -1;
	int isLeaf;
	int SerialNum = 0;
	glm::vec3 AA, BB;
};

struct BVHNodeEncoded {
	glm::vec3 childs = glm::vec3(-1, -1, -1);// (left, right, ±£¡Ù)
	glm::vec3 leafInfo = glm::vec3(-1, -1, -1); // (n, index, isLeaf)
	glm::vec3 AA, BB;
};



class BVH
{
public:
	BVH()= default;
	~BVH()= default;

	BVHNode* buildBVH(std::vector<Triangle>& triangles, int left, int right);
	//BVHNode* buildBVHwithSAH(std::vector<Triangle>& triangles, int left, int right);

	void flattenBVHTree(BVHNode* node);

	void printBVHNodeEncodeed();

	const int GetBVHNodeNum() const { return m_BVHNodeEncodeeds.size(); }
	const void* GetBVHNodeEncodedAddr() const { return &m_BVHNodeEncodeeds[0]; }

private:
	

private:
	int leafNum = 2;
	int serialNum = 0;

	std::vector<BVHNodeEncoded> m_BVHNodeEncodeeds;
};