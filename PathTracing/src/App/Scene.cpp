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
    // Check if mesh was already loaded
    //for (int i = 0; i < m_Models.size(); i++)
    //    if (m_Models[i]->Name == filename)
    //        return i;

    id = m_Models.size();
    Model* model = new Model;

    printf("Loading model %s\n", filename.c_str());
    if (model->LoadFromFile(filename))
    {
        m_Models.push_back(model);

        for (size_t i = 0; i < model->Triangles.size(); i++)
        {
            auto& t = model->Triangles[i];
            Material& m = t.material;
            m.baseColor = glm::vec3(0.4, 0.4, 0.4);

            TriangleEncoded te;
            te.p1 = t.position[0];
            te.p2 = t.position[1];
            te.p3 = t.position[2];
            te.n3 = t.normal[0];
            te.n3 = t.normal[1];
            te.n3 = t.normal[2];
            te.emissive = m.emissive;
            te.baseColor = m.baseColor;
            te.param1 = glm::vec3(m.subsurface, m.metallic, m.specular);
            te.param2 = glm::vec3(m.specularTint, m.roughness, m.anisotropic);
            te.param3 = glm::vec3(m.sheen, m.sheenTint, m.clearcoat);
            te.param4 = glm::vec3(m.clearcoatGloss, m.IOR, m.transmission);

            m_Triangles.push_back(t);
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
