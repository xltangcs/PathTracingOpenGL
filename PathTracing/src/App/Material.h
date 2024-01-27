#pragma once

#include <glm/glm.hpp>
// 物体表面材质定义
struct Material {
    glm::vec3 emissive = glm::vec3(0, 0, 0);  // 作为光源时的发光颜色
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

/*
* https://media.disneyanimation.com/uploads/production/publication_asset/48/asset/s2012_pbs_disney_brdf_notes_v3.pdf
subsurface 次表面散射参数，用于将次表面散射和漫反射插值, 基于经验公式凑出来的
metallic 金属度，决定了漫反射的比例（金属不发生漫反射）
specular 镜面反射强度控制
specularTint ：控制镜面反射的颜色，根据该参数，在 baseColor 和 vec3(1) 之间插值 specularTint越大越接近baseColor
roughness 粗糙度
anisotropic 各向异性
sheen 模拟织物布料边缘的透光
sheenTint 和 specularTint 类似，控制织物高光颜色在 baseColor 和 vec3(1) 之间插值
clearcoat 清漆强度，模拟粗糙物体表面的光滑涂层（比如木地板） ，和 specular 一样，不过清漆的值比较弱
clearcoatGloss 清漆的 “粗糙度”，或者说光泽程度。事实上 Disney 的代码把该参数直接作为粗糙度
IOR
transmission
*/