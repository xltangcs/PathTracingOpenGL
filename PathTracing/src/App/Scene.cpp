#include "Scene.h"

//#include <algorithm>
//#include <iostream>

#include <glad/glad.h>

int Scene::AddModel(const std::string& filename)
{
    int id = -1;
    //Check if mesh was already loaded
    for (int i = 0; i < m_Models.size(); i++)
        if (m_Models[i]->Name == filename)
            return i;

    id = m_Models.size();
    Model* model = new Model;

    //printf("Loading model %s\n", filename.c_str());
    if (model->LoadFromFile(filename))
    {
        m_Models.push_back(model);
    }
    else
    {
        printf("Unable to load model %s\n", filename.c_str());
        delete model;
        id = -1;
    }
    return id;
}

int Scene::AddMaterial(const Material& material)
{
    int id = m_Materials.size();
    m_Materials.push_back(material);
    return id;
}

//int Scene::AddLight(const Light& light)
//{
//    int id = m_Lights.size();
//    m_Lights.push_back(light);
//    return id;
//}

int Scene::AddModelInstance(const ModelInstance& instances)
{
    int id = m_Instances.size();
    m_Instances.push_back(instances);
    return 0;
}

void Scene::AddEnvMap(const std::string& filename)
{
    HDRLoaderResult hdrRes;
    bool r = HDRLoader::load(filename.c_str(), hdrRes);
    if (!r)
    {
        printf("Failed to load %s\n", filename.c_str());
        return;
    }
    glGenTextures(1, &envMapID);
    glBindTexture(GL_TEXTURE_2D, envMapID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, hdrRes.width, hdrRes.height, 0, GL_RGB, GL_FLOAT, hdrRes.cols);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, hdrRes.width, hdrRes.height, 0, GL_RGB, GL_FLOAT, hdrRes.cols);
}

void Scene::ProcessScene()
{
    printf("Processing Scene ...\n");
    for (auto instance : m_Instances)
    {
        Model* model = m_Models[instance.modelID];
        Material& m = m_Materials[instance.materialID];
        glm::mat4 trans = instance.transform;

        for (size_t i = 0; i < model->Triangles.size(); i++)
        {
            Triangle& t = model->Triangles[i];

            TriangleEncoded te;

            te.p1 = glm::vec3(trans * glm::vec4(t.position[0], 1.0f));
            te.p2 = glm::vec3(trans * glm::vec4(t.position[1], 1.0f));
            te.p3 = glm::vec3(trans * glm::vec4(t.position[2], 1.0f));
            te.n1 = glm::vec3(trans * glm::vec4(t.normal[0], 1.0f));
            te.n2 = glm::vec3(trans * glm::vec4(t.normal[1], 1.0f));
            te.n3 = glm::vec3(trans * glm::vec4(t.normal[2], 1.0f));

            te.emissive = m.emissive;
            te.baseColor = m.baseColor;
            te.param1 = glm::vec3(m.subsurface, m.metallic, m.specular);
            te.param2 = glm::vec3(m.specularTint, m.roughness, m.anisotropic);
            te.param3 = glm::vec3(m.sheen, m.sheenTint, m.clearcoat);
            te.param4 = glm::vec3(m.clearcoatGloss, m.IOR, m.transmission);

            m_TriangleEncoded.push_back(te);
        }
    } 
}
