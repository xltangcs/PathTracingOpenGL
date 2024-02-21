#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "App/Model.h"
#include "App/Material.h"

//struct Light
//{
//	glm::vec3 position;
//	glm::vec3 emission;
//	glm::vec3 u;
//	glm::vec3 v;
//	float radius;
//	float area;
//	float type;
//};

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
	char* sceneName;
	std::vector<TriangleEncoded> m_TriangleEncoded;
public:
	Scene(char* name = "Scene") : sceneName(name) {};
	~Scene() = default;

	int AddModel(const std::string& filename);
	int AddMaterial(const Material& material);
	//int AddLight(const Light& light);
	int AddModelInstance(const ModelInstance& instances);

	void ProcessScene();
private:
	std::vector<Model*> m_Models;
	std::vector<Material> m_Materials;
	//std::vector<Light> m_Lights;
	std::vector<ModelInstance> m_Instances;
};