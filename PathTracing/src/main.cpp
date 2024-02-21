#include <Windows.h>
#include <cstdio>

#include <memory>
#include <vector>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Application.h"
#include "Core/Image.h"
#include "App/Camera.h"
#include "App/PathTracing.h"

class MyImGuiLayer : public ImGuiLayer
{
public:
	MyImGuiLayer()
		:m_Camera(45.0f, 0.1f, 100.0f)
	{
		auto pathtracing = std::make_shared<PathTracing>();
		m_Renderer.emplace_back(pathtracing);

		for (auto render : m_Renderer)
			m_RendererName.emplace_back(render->GetRendererName());
	}

	virtual void OnUpdate(float ts) override
	{
		int width, height;
		auto window = Application::Get().GetGLFWwindow();
		//glfwGetWindowSize(window, &width, &height);
		glfwGetFramebufferSize(window, &width, &height);
		m_Width = width; m_Height = height;

		m_Camera.OnUpdate(ts);
		m_Renderer[m_CurrentIndex]->OnResize(m_Width, m_Height);
	}

	virtual void ShowUI(float ts) override
	{
		ImGui::Begin("Setting");

		ImGui::Text("The average fps: %.3f", ImGui::GetIO().Framerate);

		if (ImGui::Combo("Scene", &m_RendererIndex, m_RendererName.data(), m_Renderer.size()))
		{
			m_CurrentIndex = m_RendererIndex;
		}
		ImGui::Separator();
		ImGui::DragFloat3("Camera Position", glm::value_ptr(m_Camera.GetPosition()));
		ImGui::DragFloat3("Camera Direction", glm::value_ptr(m_Camera.GetDirection()));
		ImGui::Checkbox("Camera Rotation", &m_Camera.GetIsRotation());

		ImGui::End();
	}

	virtual void Render(float ts) override
	{
		m_Renderer[m_CurrentIndex]->Render(m_Camera);
	}

private:
	unsigned int m_Width = 0, m_Height = 0;

	std::vector<std::shared_ptr<Renderer>> m_Renderer;
	std::vector<const char*> m_RendererName;



	int m_RendererIndex = 0;
	int m_CurrentIndex = 0;
	Camera m_Camera;
};


int main()
{
	static Application* RayTracing = new Application("PathTracing");
	std::shared_ptr<MyImGuiLayer> myimguilayer = std::make_shared<MyImGuiLayer>();

	RayTracing->PushImGuiLayer(myimguilayer);
	RayTracing->Run();

	return 0;
}