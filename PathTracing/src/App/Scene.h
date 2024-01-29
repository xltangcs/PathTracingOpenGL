#pragma once

#include "App/Model.h"


class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	int AddModel(const std::string& filename);

	const int GetTriangleNum() const { return m_TriangleEncodeds.size(); }
	const void* GetTriangleAddr() const { return &m_TriangleEncodeds[0]; }
	std::vector<Triangle>& GetTriangles() { return m_Triangles; }

private:
	int leafNum = 2;

	std::vector<Model*> m_Models;
	std::vector<Triangle> m_Triangles;
	std::vector<TriangleEncoded> m_TriangleEncodeds;
};