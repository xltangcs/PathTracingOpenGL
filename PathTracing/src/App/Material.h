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

/*
* https://media.disneyanimation.com/uploads/production/publication_asset/48/asset/s2012_pbs_disney_brdf_notes_v3.pdf
subsurface �α���ɢ����������ڽ��α���ɢ����������ֵ, ���ھ��鹫ʽ�ճ�����
metallic �����ȣ�������������ı��������������������䣩
specular ���淴��ǿ�ȿ���
specularTint �����ƾ��淴�����ɫ�����ݸò������� baseColor �� vec3(1) ֮���ֵ specularTintԽ��Խ�ӽ�baseColor
roughness �ֲڶ�
anisotropic ��������
sheen ģ��֯�ﲼ�ϱ�Ե��͸��
sheenTint �� specularTint ���ƣ�����֯��߹���ɫ�� baseColor �� vec3(1) ֮���ֵ
clearcoat ����ǿ�ȣ�ģ��ֲ��������Ĺ⻬Ϳ�㣨����ľ�ذ壩 ���� specular һ�������������ֵ�Ƚ���
clearcoatGloss ����� ���ֲڶȡ�������˵����̶ȡ���ʵ�� Disney �Ĵ���Ѹò���ֱ����Ϊ�ֲڶ�
IOR
transmission
*/