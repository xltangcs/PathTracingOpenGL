#pragma once
#include "App/Scene.h"

Scene CornellBoxScene()
{
	Scene Scene("Cornell Box Scene");

	int cbox_ceiling_id		= Scene.AddModel("./assets/models/cornell_box/cbox_ceiling.obj");
	int cbox_floor_id		= Scene.AddModel("./assets/models/cornell_box/cbox_floor.obj");
	int cbox_back_id		= Scene.AddModel("./assets/models/cornell_box/cbox_back.obj");
	int cbox_redwall_id		= Scene.AddModel("./assets/models/cornell_box/cbox_left.obj");
	int cbox_greenwall_id	= Scene.AddModel("./assets/models/cornell_box/cbox_right.obj");
	int cube_id	= Scene.AddModel("./assets/models/cornell_box/Cube.obj");

	Material white;
	white.baseColor = glm::vec3(0.725f, 0.71f, 0.68f);

	Material red;
	red.baseColor = glm::vec3(0.63f, 0.065f, 0.05f);

	Material green;
	green.baseColor = glm::vec3(0.14f, 0.45f, 0.091f);

	Material light;
	light.emissive = glm::vec3(17,12,4);

	int white_mat_id	= Scene.AddMaterial(white);
	int red_mat_id		= Scene.AddMaterial(red);
	int green_mat_id	= Scene.AddMaterial(green);
	int light_mat_id    = Scene.AddMaterial(light);

	glm::mat4 trans = glm::mat4(1.0f);

	glm::mat4 cbox_ceiling_trans	= glm::translate(trans, glm::vec3(0.0f, 5.0f, -10.0f))  * glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 cbox_floor_trans		= glm::translate(trans, glm::vec3(0.0f, -5.0f, -10.0f)) * glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 cbox_back_trans		= glm::translate(trans, glm::vec3(0.0f, 0.0f, -15.0f)) * glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 cbox_redwall_trans	= glm::translate(trans, glm::vec3(-5.0f, 0.0f, -10.0f)) * glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 cbox_greenwall_trans	= glm::translate(trans, glm::vec3(5.0f, 0.0f, -10.0f))  * glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

	glm::mat4 cbox_largebox_trans = glm::translate(trans, glm::vec3(-2.5f, -1.5f, -11.0f)) * glm::rotate(trans, glm::radians(35.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(trans, glm::vec3(2.5f, 7.0f, 1.0f));
	glm::mat4 cbox_smallbox_trans = glm::translate(trans, glm::vec3(2.5f, -3.5f, -8.0f)) * glm::rotate(trans, glm::radians(65.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(trans, glm::vec3(3.0f, 3.0f, 3.0f));

	glm::mat4 light_trans			= glm::translate(trans, glm::vec3(0.0f, 4.9f, -10.0f)) * glm::scale(trans, glm::vec3(0.3f, 0.3f, 0.3f));

	ModelInstance instance1("Ceiling   ", cbox_ceiling_id	, cbox_ceiling_trans	, white_mat_id);
	ModelInstance instance2("Floor     ", cbox_floor_id		, cbox_floor_trans		, white_mat_id);
	ModelInstance instance3("Back Wall ", cbox_back_id		, cbox_back_trans		, white_mat_id);
	ModelInstance instance4("Left Wall ", cbox_redwall_id	, cbox_redwall_trans	, red_mat_id);
	ModelInstance instance5("Right Wall", cbox_greenwall_id	, cbox_greenwall_trans	, green_mat_id);
	ModelInstance instance6("Large Box ", cube_id			, cbox_largebox_trans	, white_mat_id);
	ModelInstance instance7("Small Box ", cube_id			, cbox_smallbox_trans	, white_mat_id);
	ModelInstance instance8("light", cbox_ceiling_id		, light_trans			, light_mat_id);

	Scene.AddModelInstance(instance1);
	Scene.AddModelInstance(instance2);
	Scene.AddModelInstance(instance3);
	Scene.AddModelInstance(instance4);
	Scene.AddModelInstance(instance5);
	Scene.AddModelInstance(instance6);
	Scene.AddModelInstance(instance7);
	Scene.AddModelInstance(instance8);


	Scene.ProcessScene();
	return Scene;
}