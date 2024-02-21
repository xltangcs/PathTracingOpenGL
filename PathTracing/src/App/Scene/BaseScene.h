#pragma once

#include "App/Scene.h"

Scene CreatBaseScene()
{
	//// hdr È«¾°Í¼
	//HDRLoaderResult hdrRes;
	//bool r = HDRLoader::load("./assets/textures/HDR/sunset.hdr", hdrRes);
	//hdrMap = getTextureRGB32F(hdrRes.width, hdrRes.height);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, hdrRes.width, hdrRes.height, 0, GL_RGB, GL_FLOAT, hdrRes.cols);

	Scene m_Scene("Base Scene");

	int BunnyID = m_Scene.AddModel("./assets/models/Stanford Bunny.obj");
	Material BunnyMat;
	BunnyMat.baseColor = glm::vec3(1, 0, 0);
	int BunnyMatId = m_Scene.AddMaterial(BunnyMat);
	glm::mat4 BunnyTrans = glm::mat4(1.0f);
	BunnyTrans = glm::translate(BunnyTrans, glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(BunnyTrans, glm::vec3(4.0f, 4.0f, 4.0f));
	ModelInstance Bunny("Bunny", BunnyID, BunnyTrans, BunnyMatId);
	m_Scene.AddModelInstance(Bunny);


	int quadId = m_Scene.AddModel("./assets/models/Cube.obj");
	Material blueColor;
	blueColor.baseColor = glm::vec3(0.725, 0.71, 0.68);
	//blueColor.emissive = glm::vec3(30, 20, 10);
	int blueMatId = m_Scene.AddMaterial(blueColor);
	glm::mat4 quadTrans = glm::mat4(1.0f);
	quadTrans = glm::translate(quadTrans, glm::vec3(0.0f, -1.0f, 0.0f)) * glm::scale(quadTrans, glm::vec3(5.0f, 0.01f, 5.0f));
	ModelInstance quad("Cube", quadId, quadTrans, blueMatId);
	m_Scene.AddModelInstance(quad);


	int shpereId = m_Scene.AddModel("./assets/models/Sphere.obj");
	Material whiteColor;
	whiteColor.baseColor = glm::vec3(1, 1, 1);
	whiteColor.emissive = glm::vec3(30, 20, 10);
	int whiteMatId = m_Scene.AddMaterial(whiteColor);
	glm::mat4 sphereTrans = glm::mat4(1.0f);
	sphereTrans = glm::translate(sphereTrans, glm::vec3(0.0f, 1.5f, 0.0f)) * glm::scale(sphereTrans, glm::vec3(0.1, 0.1, 0.1));
	ModelInstance sphere("Sphere", shpereId, sphereTrans, whiteMatId);
	m_Scene.AddModelInstance(sphere);

	m_Scene.ProcessScene();
	return m_Scene;
}