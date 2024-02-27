#pragma once
#include "App/Scene.h"

Scene Scene_Template()
{
	Scene scene("Scene Template");

	//model
	int cbox_ceiling_id = scene.AddModel("./assets/models/cornell_box/cbox_ceiling.obj");

	//material
	Material white;
	white.baseColor = glm::vec3(0.725f, 0.71f, 0.68f);

	int white_mat_id = scene.AddMaterial(white);

	//trans
	glm::mat4 trans = glm::mat4(1.0f);

	glm::mat4 cbox_ceiling_trans = glm::translate(trans, glm::vec3(-2.5f, -1.5f, -11.0f)) * glm::rotate(trans, glm::radians(35.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(trans, glm::vec3(2.5f, 7.0f, 1.0f));

	//model instance
	ModelInstance instance1("Ceiling   ", cbox_ceiling_id, cbox_ceiling_trans, white_mat_id);

	scene.AddModelInstance(instance1);

	//process
	scene.ProcessScene();
	return scene;
}