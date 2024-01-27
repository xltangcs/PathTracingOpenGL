#pragma once

#include "App/Model.h"

// BVH ���ڵ�
struct BVHNode {
	int left, right;    // ������������
	int n, index;       // Ҷ�ӽڵ���Ϣ               
	glm::vec3 AA, BB;        // ��ײ��
};

struct BVHNodeEncoded {
	glm::vec3 childs;// (left, right, ����)
	glm::vec3 leafInfo;
	glm::vec3 AA, BB;
};

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	int AddModel(const std::string& filename);
	int buildBVH(int l, int r);
	int buildBVHwithSAH(int l, int r);

	void translateTriangleEncode();

	const int TriangleNum() const { return m_Triangles.size(); }
	const int BVHNodeNum() const { return m_BVHNodes.size(); }
	const void* GetBVHNodeAddr() const { return &m_BVHNodeEncodeds[0]; }
	const void* GetTriangleAddr() const { return &m_TriangleEncodeds[0]; }

private:
	int leafNum = 8;

	std::vector<Model*> m_Models;
	std::vector<Triangle> m_Triangles;
	std::vector<TriangleEncoded> m_TriangleEncodeds;
	std::vector<BVHNode> m_BVHNodes;
	std::vector<BVHNodeEncoded> m_BVHNodeEncodeds;
};