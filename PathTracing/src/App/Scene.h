#pragma once

#include "App/Model.h"
#include "App/Material.h"

struct Light
{
	glm::vec3 position;
	glm::vec3 emission;
	glm::vec3 u;
	glm::vec3 v;
	float radius;
	float area;
	float type;
};

struct ModelInstance
{
	ModelInstance(std::string name, int modelId, glm::mat4 xform, int matId)
		: name(name)
		, modelID(modelId)
		, transform(xform)
		, materialID(matId)
	{
	}
	~ModelInstance() {}

	glm::mat4 transform;
	std::string name;

	int materialID;
	int modelID;
};

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	int AddModel(const std::string& filename);
	int AddMaterial(const Material& material);
	int AddLight(const Light& light);
	int AddModelInstance(const ModelInstance& instances);

	void ProcessScene(std::vector<TriangleEncoded>& triangleEncodeds);

	const int GetTriangleNum() const { return m_TriangleEncodeds.size(); }
	const void* GetTriangleAddr() const { return &m_TriangleEncodeds[0]; }
	std::vector<Triangle>& GetTriangles() { return m_Triangles; }
private:

	std::vector<Model*> m_Models;
	std::vector<Material> m_Materials;
	std::vector<Light> m_Lights;
	std::vector<ModelInstance> m_Instances;

	std::vector<TriangleEncoded> m_TriangleEncodeds;
	std::vector<Triangle> m_Triangles;
};