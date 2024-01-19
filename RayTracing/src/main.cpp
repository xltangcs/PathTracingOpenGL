#include <memory>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Application.h"
#include "Core/Image.h"
#include "RayTracing/Renderer.h"
#include "RayTracing/Camera.h"


class MyImGuiLayer : public ImGuiLayer
{
public:
	MyImGuiLayer()
		:m_Camera(45.0f, 0.1f, 100.0f)
	{

	}

	virtual void Render(float ts) override
	{
		//m_Renderer.RenderCube(m_Camera);
		m_Renderer.RenderImage(m_Camera);
	}

	virtual void ShowUI(float ts) override
	{
		ImGui::Begin("Setting");
		
		ImGui::DragFloat3("Camera Position", glm::value_ptr(m_Camera.GetPosition()));
		ImGui::DragFloat3("Camera Direction", glm::value_ptr(m_Camera.GetDirection()));
		ImGui::Checkbox("Camera Rotation", &m_Camera.GetIsRotation());

		ImGui::End();
	}

	virtual void OnUpdate(float ts) override
	{
		m_Camera.OnUpdate(ts);
		//m_Renderer.OnResize(m_ViewportSize.x, m_ViewportSize.y);
	}

private:
	Renderer m_Renderer;
	Camera m_Camera;
};


int main()
{
	static Application* RayTracing = new Application("RayTracing"); //modify
	std::shared_ptr<MyImGuiLayer> myimguilayer = std::make_shared<MyImGuiLayer>();

	RayTracing->PushImGuiLayer(myimguilayer);
	RayTracing->Run();

	return 0;
}