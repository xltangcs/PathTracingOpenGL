#pragma once

#include <glm/glm.hpp>
// ���������ʶ���
struct Material {
    glm::vec3 emissive = glm::vec3(0, 0, 0);  // ��Ϊ��Դʱ�ķ�����ɫ
    glm::vec3 baseColor = glm::vec3(1, 1, 1);
    float subsurface = 0.0;
    float metallic = 0.0;
    float specular = 0.0;
    float specularTint = 0.0;
    float roughness = 0.0;
    float anisotropic = 0.0;
    float sheen = 0.0;
    float sheenTint = 0.0;
    float clearcoat = 0.0;
    float clearcoatGloss = 0.0;
    float IOR = 1.0;
    float transmission = 0.0;
};