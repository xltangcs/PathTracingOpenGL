#pragma once
#include "App/Scene.h"

Scene ImportanceSampling()
{
	Scene scene("Importance_Sampling");


	int teapot_id = scene.AddModel("./assets/models/teapot.obj");
	int cube_id = scene.AddModel("./assets/models/Cube.obj");


	Material teapot_mat;
	teapot_mat.roughness = 0.5;
	teapot_mat.specular = 1.0;
	teapot_mat.metallic = 1.0;
	teapot_mat.clearcoat = 1.0;
	teapot_mat.clearcoatGloss = 0.0;
	teapot_mat.baseColor = glm::vec3(1, 0.73, 0.25);

	Material cube_mat;
	cube_mat.roughness = 0.01;
	cube_mat.metallic = 0.1;
	cube_mat.specular = 1.0;
	cube_mat.baseColor = glm::vec3(1, 1, 1);

	int teapot_mat_id = scene.AddMaterial(teapot_mat);
	int cube_mat_id = scene.AddMaterial(cube_mat);

	glm::mat4 trans = glm::mat4(1.0f);

	glm::mat4 teapot_trans = glm::translate(trans, glm::vec3(0.0f, -2.0f, -5.0f))
		* glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::scale(trans, glm::vec3(0.4f, 0.4f, 0.4f));

	glm::mat4 cube_trans = glm::translate(trans, glm::vec3(0.0f, -3.0f, -10.0f))
		//* glm::rotate(quadTrans, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::scale(trans, glm::vec3(100.0f, 2.0f, 100.0f));


	ModelInstance teapot("teapot", teapot_id, teapot_trans, teapot_mat_id);
	ModelInstance cube("teapot", cube_id, cube_trans, cube_mat_id);

	scene.AddModelInstance(teapot);
	scene.AddModelInstance(cube);

	scene.AddEnvMap("./assets/textures/HDR/peppermint_powerplant_4k.hdr");


	//process
	scene.ProcessScene();
	return scene;
}