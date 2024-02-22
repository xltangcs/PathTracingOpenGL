#pragma once
#include "App/Scene.h"

Scene BRDF_Test_Scene()
{
	Scene scene("BRDF Test Scene");

	int cbox_ceiling_id = scene.AddModel("./assets/models/cornell_box/cbox_ceiling.obj");
	int cbox_floor_id = scene.AddModel("./assets/models/cornell_box/cbox_floor.obj");
	int cbox_back_id = scene.AddModel("./assets/models/cornell_box/cbox_back.obj");
	int cbox_redwall_id = scene.AddModel("./assets/models/cornell_box/cbox_left.obj");
	int cbox_greenwall_id = scene.AddModel("./assets/models/cornell_box/cbox_right.obj");
	int sphere_id = scene.AddModel("./assets/models/Sphere.obj");

	Material white;
	white.baseColor = glm::vec3(0.725f, 0.71f, 0.68f);

	Material red;
	red.baseColor = glm::vec3(0.63f, 0.065f, 0.05f);

	Material green;
	green.baseColor = glm::vec3(0.14f, 0.45f, 0.091f);

	Material light;
	light.emissive = glm::vec3(17, 12, 4);

	//Material green;
	//green.baseColor = glm::vec3(0.14f, 0.45f, 0.091f);


	int white_mat_id = scene.AddMaterial(white);
	int red_mat_id = scene.AddMaterial(red);
	int green_mat_id = scene.AddMaterial(green);
	int light_mat_id = scene.AddMaterial(light);

	glm::mat4 trans = glm::mat4(1.0f);

	glm::mat4 cbox_ceiling_trans = glm::translate(trans, glm::vec3(0.0f, 5.0f, -10.0f)) * glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 cbox_floor_trans = glm::translate(trans, glm::vec3(0.0f, -5.0f, -10.0f)) * glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	
	glm::mat4 cbox_back_trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, -10.0f)) 
		* glm::rotate(trans, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::scale(trans, glm::vec3(10.5f, 0.5f, 0.5f));
	
	glm::mat4 cbox_redwall_trans = glm::translate(trans, glm::vec3(-5.0f, 0.0f, -10.0f)) * glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 cbox_greenwall_trans = glm::translate(trans, glm::vec3(5.0f, 0.0f, -10.0f)) * glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 light_trans = glm::translate(trans, glm::vec3(0.0f, 4.99f, -8.0f)) 
		//* glm::rotate(trans, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::scale(trans, glm::vec3(0.25f, 0.25f, 0.25f));

	ModelInstance instance1("Ceiling   ", cbox_ceiling_id, cbox_ceiling_trans, white_mat_id);
	ModelInstance instance2("Floor     ", cbox_floor_id, cbox_floor_trans, white_mat_id);
	ModelInstance instance3("Back Wall ", cbox_back_id, cbox_back_trans, white_mat_id);
	ModelInstance instance4("Left Wall ", cbox_redwall_id, cbox_redwall_trans, red_mat_id);
	ModelInstance instance5("Right Wall", cbox_greenwall_id, cbox_greenwall_trans, green_mat_id);
	ModelInstance instance6("light", cbox_ceiling_id, light_trans, light_mat_id);

	//scene.AddModelInstance(instance1);
	//scene.AddModelInstance(instance2);
	scene.AddModelInstance(instance3);
	//scene.AddModelInstance(instance4);
	//scene.AddModelInstance(instance5);
	scene.AddModelInstance(instance6);

	//scene.AddEnvMap("./assets/textures/HDR/sunset.hdr");

	scene.ProcessScene();
	return scene;
}