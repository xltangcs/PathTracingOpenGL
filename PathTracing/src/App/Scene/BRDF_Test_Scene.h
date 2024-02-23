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
	int teapot_id = scene.AddModel("./assets/models/teapot.obj");

	Material white;
	white.baseColor = glm::vec3(0.725f, 0.71f, 0.68f);

	Material red;
	red.baseColor = glm::vec3(0.63f, 0.065f, 0.05f);

	Material green;
	green.baseColor = glm::vec3(0.14f, 0.45f, 0.091f);

	Material light_mat;
	light_mat.emissive = glm::vec3(17, 12, 4);

	//different roughness
	Material m1;
	m1.baseColor = glm::vec3(1.0f, 0.5f, 0.5f);
	m1.roughness = 0.1f;
	m1.metallic = 1.0f;

	Material m2;
	m2.baseColor = glm::vec3(1.0f, 0.5f, 0.5f);
	m2.roughness = 0.2f;
	m2.metallic = 1.0f;

	Material m3;
	m3.baseColor = glm::vec3(1.0f, 0.5f, 0.5f);
	m3.roughness = 0.3f;
	m3.metallic = 1.0f;


	//different metallic
	Material m4;
	m4.baseColor = glm::vec3(1.0f, 0.5f, 0.5f);
	m4.roughness = 0.1f;
	m4.metallic = 0.3f;

	Material m5;
	m5.baseColor = glm::vec3(1.0f, 0.5f, 0.5f);
	m5.roughness = 0.1f;
	m5.metallic = 0.6f;

	Material m6;
	m6.baseColor = glm::vec3(1.0f, 0.5f, 0.5f);
	m6.roughness = 0.1f;
	m6.metallic = 0.9f;

	Material m7;
	m7.baseColor = glm::vec3(0.75f, 0.7f, 0.15f);
	m7.roughness = 0.15f;
	m7.metallic = 1.0f;
	m7.clearcoat = 1.0f;

	int white_mat_id = scene.AddMaterial(white);
	int red_mat_id = scene.AddMaterial(red);
	int green_mat_id = scene.AddMaterial(green);
	int light_mat_id = scene.AddMaterial(light_mat);

	int m1_mat_id = scene.AddMaterial(m1);
	int m2_mat_id = scene.AddMaterial(m2);
	int m3_mat_id = scene.AddMaterial(m3);
	int m4_mat_id = scene.AddMaterial(m4);
	int m5_mat_id = scene.AddMaterial(m5);
	int m6_mat_id = scene.AddMaterial(m6);
	int m7_mat_id = scene.AddMaterial(m7);

	glm::mat4 trans = glm::mat4(1.0f);

	glm::mat4 cbox_ceiling_trans = glm::translate(trans, glm::vec3(0.0f, 5.0f, -10.0f)) * glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 cbox_floor_trans = glm::translate(trans, glm::vec3(0.0f, -5.0f, -10.0f)) * glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));	
	glm::mat4 cbox_back_trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, -15.0f)) * glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 cbox_redwall_trans = glm::translate(trans, glm::vec3(-5.0f, 0.0f, -10.0f)) * glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 cbox_greenwall_trans = glm::translate(trans, glm::vec3(5.0f, 0.0f, -10.0f)) * glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 light_trans = glm::translate(trans, glm::vec3(0.0f, 4.99f, -10.0f)) * glm::scale(trans, glm::vec3(0.25f, 0.25f, 0.25f));

	glm::mat4 sphere1_trans = glm::translate(trans, glm::vec3(-2.0f, 0.0f, -10.0f)) * glm::scale(trans, glm::vec3(0.35f, 0.35f, 0.35f));
	glm::mat4 sphere2_trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, -10.0f)) * glm::scale(trans, glm::vec3(0.35f, 0.35f, 0.35f));
	glm::mat4 sphere3_trans = glm::translate(trans, glm::vec3(2.0f, 0.0f, -10.0f)) * glm::scale(trans, glm::vec3(0.35f, 0.35f, 0.35f));
	glm::mat4 sphere4_trans = glm::translate(trans, glm::vec3(-2.0f, -2.0f, -8.0f)) * glm::scale(trans, glm::vec3(0.35f, 0.35f, 0.35f));
	glm::mat4 sphere5_trans = glm::translate(trans, glm::vec3(0.0f, -2.0f, -8.0f)) * glm::scale(trans, glm::vec3(0.35f, 0.35f, 0.35f));
	glm::mat4 sphere6_trans = glm::translate(trans, glm::vec3(2.0f, -2.0f, -8.0f)) * glm::scale(trans, glm::vec3(0.35f, 0.35f, 0.35f));
	
	glm::mat4 teapot_trans = glm::translate(trans, glm::vec3(-2.0f, -5.0f, -6.0f)) 
		* glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::scale(trans, glm::vec3(0.35f, 0.35f, 0.35f));

	ModelInstance instance1("Ceiling   ", cbox_ceiling_id, cbox_ceiling_trans, white_mat_id);
	ModelInstance instance2("Floor     ", cbox_floor_id, cbox_floor_trans, white_mat_id);
	ModelInstance instance3("Back Wall ", cbox_back_id, cbox_back_trans, white_mat_id);
	ModelInstance instance4("Left Wall ", cbox_redwall_id, cbox_redwall_trans, red_mat_id);
	ModelInstance instance5("Right Wall", cbox_greenwall_id, cbox_greenwall_trans, green_mat_id);
	ModelInstance light("light", cbox_ceiling_id, light_trans, light_mat_id);

	ModelInstance sphere1("sphere1", sphere_id, sphere1_trans, m1_mat_id);
	ModelInstance sphere2("sphere2", sphere_id, sphere2_trans, m2_mat_id);
	ModelInstance sphere3("sphere3", sphere_id, sphere3_trans, m3_mat_id);
	ModelInstance sphere4("sphere4", sphere_id, sphere4_trans, m4_mat_id);
	ModelInstance sphere5("sphere5", sphere_id, sphere5_trans, m5_mat_id);
	ModelInstance sphere6("sphere6", sphere_id, sphere6_trans, m6_mat_id);

	ModelInstance teapot("teapot", teapot_id, teapot_trans, m7_mat_id);

	//scene.AddModelInstance(instance1);
	//scene.AddModelInstance(instance2);
	//scene.AddModelInstance(instance3);
	//scene.AddModelInstance(instance4);
	//scene.AddModelInstance(instance5);

	//scene.AddModelInstance(light);

	scene.AddModelInstance(sphere1);
	scene.AddModelInstance(sphere2);
	scene.AddModelInstance(sphere3);
	scene.AddModelInstance(sphere4);
	scene.AddModelInstance(sphere5);
	scene.AddModelInstance(sphere6);

	scene.AddModelInstance(teapot);

	scene.AddEnvMap("./assets/textures/HDR/sunset.hdr");

	scene.ProcessScene();
	return scene;
}