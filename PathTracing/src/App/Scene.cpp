#include "Scene.h"

#include <algorithm>
#include <iostream>

const float INF = 0x3f3f3f3f;

// ----------------------------------------------------------------------------- //
//
// 按照三角形中心排序 -- 比较函数
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

int Scene::AddModel(const std::string& filename)
{
    int id = -1;
    //Check if mesh was already loaded
    for (int i = 0; i < m_Models.size(); i++)
        if (m_Models[i]->Name == filename)
            return i;

    id = m_Models.size();
    Model* model = new Model;

    printf("Loading model %s\n", filename.c_str());
    if (model->LoadFromFile(filename))
    {
        m_Models.push_back(model);

        for (size_t i = 0; i < model->Triangles.size(); i++)
        {
            auto& t = model->Triangles[i];
            m_Triangles.push_back(t);

            Material& m = t.material;
            m.baseColor = glm::vec3(0.4, 0.4, 0.4);

            TriangleEncoded te;

            te.p1 = t.position[0];
            te.p2 = t.position[1];
            te.p3 = t.position[2];
            te.n1 = t.position[0];
            te.n2 = t.position[1];
            te.n3 = t.position[2];



            te.emissive = m.emissive;
            te.baseColor = m.baseColor;
            te.param1 = glm::vec3(m.subsurface, m.metallic, m.specular);
            te.param2 = glm::vec3(m.specularTint, m.roughness, m.anisotropic);
            te.param3 = glm::vec3(m.sheen, m.sheenTint, m.clearcoat);
            te.param4 = glm::vec3(m.clearcoatGloss, m.IOR, m.transmission);

           
            m_TriangleEncodeds.push_back(te);

        }
    }
    else
    {
        printf("Unable to load model %s\n", filename.c_str());
        delete model;
        id = -1;
    }
    //std::cout << "Triangles : " << m_Triangles.size() << std::endl;
    //std::cout << "Triangle encode : " << m_TriangleEncodeds.size() << std::endl;

    return id;
}

int Scene::AddMaterial(const Material& material)
{
    int id = m_Materials.size();
    m_Materials.push_back(material);
    return id;
}

int Scene::AddLight(const Light& light)
{
    int id = m_Lights.size();
    m_Lights.push_back(light);
    return id;
}

int Scene::AddModelInstance(const ModelInstance& instances)
{
    int id = m_Instances.size();
    m_Instances.push_back(instances);
    return 0;
}

/*void Scene::ProcessScene(std::vector<TriangleEncoded>& triangleEncodeds)
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
            for (int i = 0; i < 3; i++)
            {
                glm::vec4 pos = glm::vec4(t.position[i], 1.0f);
                glm::vec4 nor = glm::vec4(t.normal[i], 1.0f);
                pos *= trans;
                nor *= trans;
            }
            

            te.p1 = pos.x;
            te.p2 = pos.y;
            te.p3 = pos.z;
            te.n3 = t.normal[0];
            te.n3 = t.normal[1];
            te.n3 = t.normal[2];


            te.emissive = m.emissive;
            te.baseColor = m.baseColor;
            te.param1 = glm::vec3(m.subsurface, m.metallic, m.specular);
            te.param2 = glm::vec3(m.specularTint, m.roughness, m.anisotropic);
            te.param3 = glm::vec3(m.sheen, m.sheenTint, m.clearcoat);
            te.param4 = glm::vec3(m.clearcoatGloss, m.IOR, m.transmission);

            triangleEncodeds.push_back(te);
        }

    } 
}*/
