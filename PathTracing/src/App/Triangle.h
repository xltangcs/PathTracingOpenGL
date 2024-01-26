#pragma once

#include <glm/glm.hpp>

#include "App/Material.h"
// �����ζ���
struct Triangle {
    std::vector<glm::vec3> position;    // ��������
    std::vector<glm::vec3> normal;    // ���㷨��
    //Material material;  // ����
    Triangle(const std::vector<glm::vec3>& p, const std::vector<glm::vec3>& n) : position(p), normal(n) {}
};

struct Triangle_encoded {
    glm::vec3 p1, p2, p3;    // ��������
    glm::vec3 n1, n2, n3;    // ���㷨��
    glm::vec3 emissive;      // �Է������
    glm::vec3 baseColor;     // ��ɫ
    glm::vec3 param1;        // (subsurface, metallic, specular)
    glm::vec3 param2;        // (specularTint, roughness, anisotropic)
    glm::vec3 param3;        // (sheen, sheenTint, clearcoat)
    glm::vec3 param4;        // (clearcoatGloss, IOR, transmission)
};