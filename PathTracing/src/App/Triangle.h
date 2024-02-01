#pragma once

#include "App/Material.h"

#include <glm/glm.hpp>

// 三角形定义
struct Triangle {
    std::vector<glm::vec3> position;    // 顶点坐标
    std::vector<glm::vec3> normal;    // 顶点法线
    //Material material;  // 材质
    Triangle(const std::vector<glm::vec3>& p, const std::vector<glm::vec3>& n) : position(p), normal(n) {}
};

struct TriangleEncoded {
    //std::vector<glm::vec3> position;    // 顶点坐标
    //std::vector<glm::vec3> normal;    // 顶点法线

    glm::vec3 p1, p2, p3;    //vertex
    glm::vec3 n1, n2, n3;    //normal
    
    glm::vec3 emissive;      // 自发光参数
    glm::vec3 baseColor;     // 颜色
    glm::vec3 param1;        // (subsurface, metallic, specular)
    glm::vec3 param2;        // (specularTint, roughness, anisotropic)
    glm::vec3 param3;        // (sheen, sheenTint, clearcoat)
    glm::vec3 param4;        // (clearcoatGloss, IOR, transmission)
};
