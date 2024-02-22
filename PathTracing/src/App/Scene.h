#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "App/Model.h"
#include "App/Material.h"
#include "App/HDRLoader.h"

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
	unsigned int envMapID;
	std::vector<TriangleEncoded> m_TriangleEncoded;
public:
	Scene(char* name = "Scene") : sceneName(name) { envMapID = 0; };
	~Scene() = default;

	int AddModel(const std::string& filename);
	int AddMaterial(const Material& material);
	
	int AddModelInstance(const ModelInstance& instances);


	void AddEnvMap(const std::string& filename);

	void ProcessScene();
private:
	std::vector<Model*> m_Models;
	std::vector<Material> m_Materials;
	//std::vector<Light> m_Lights;
	std::vector<ModelInstance> m_Instances;
};