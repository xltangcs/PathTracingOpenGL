#pragma once

#include "App/Scene.h"

Scene CreatBaseScene()
{
	Scene scene("Base Scene");

	int BunnyID = scene.AddModel("./assets/models/Stanford Bunny.obj");
	Material BunnyMat;
	BunnyMat.baseColor = glm::vec3(1, 0, 0);
	int BunnyMatId = scene.AddMaterial(BunnyMat);
	glm::mat4 BunnyTrans = glm::mat4(1.0f);
	BunnyTrans = glm::translate(BunnyTrans, glm::vec3(0.0f, -0.0f, -1.0f)) * glm::scale(BunnyTrans, glm::vec3(4.0f, 4.0f, 4.0f));
	ModelInstance Bunny("Bunny", BunnyID, BunnyTrans, BunnyMatId);
	scene.AddModelInstance(Bunny);

	int quadId = scene.AddModel("./assets/models/Cube.obj");
	Material blueColor;
	blueColor.baseColor = glm::vec3(0.725, 0.71, 0.68);
	//blueColor.emissive = glm::vec3(30, 20, 10);
	int blueMatId = scene.AddMaterial(blueColor);
	glm::mat4 quadTrans = glm::mat4(1.0f);
	quadTrans = glm::translate(quadTrans, glm::vec3(0.0f, -2.0f, -1.0f)) 
		//* glm::rotate(quadTrans, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::scale(quadTrans, glm::vec3(5.0f, 2.0f, 5.0f));
	ModelInstance quad("Cube", quadId, quadTrans, blueMatId);
	scene.AddModelInstance(quad);

	int shpereId = scene.AddModel("./assets/models/Sphere.obj");
	Material whiteColor;
	whiteColor.baseColor = glm::vec3(1, 1, 1);
	whiteColor.emissive = glm::vec3(30, 20, 10);
	int whiteMatId = scene.AddMaterial(whiteColor);
	glm::mat4 sphereTrans = glm::mat4(1.0f);
	sphereTrans = glm::translate(sphereTrans, glm::vec3(0.0f, 1.5f, -2.0f)) * glm::scale(sphereTrans, glm::vec3(0.15, 0.15, 0.15));
	ModelInstance sphere("Sphere", shpereId, sphereTrans, whiteMatId);
	scene.AddModelInstance(sphere);


	scene.AddEnvMap("./assets/textures/HDR/sunset.hdr");

	scene.ProcessScene();
	return scene;
}