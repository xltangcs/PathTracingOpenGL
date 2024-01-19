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
#include "RayTracing/Camera.h"
#include "RayTracing/Renderer.h"
#include "RayTracing/Render/FirstRenderer.h"
#include "RayTracing/Render/ContainerRenderer.h"


class MyImGuiLayer : public ImGuiLayer
{
public:
	MyImGuiLayer()
		:m_Camera(45.0f, 0.1f, 100.0f)
	{
		auto first = std::make_shared<FirstRenderer>();
		m_Renderer.emplace_back(first);
		m_RendererName.emplace_back(first->GetRendererName());
		auto container = std::make_shared<ContainerRenderer>();
		m_Renderer.emplace_back(container);
		m_RendererName.emplace_back(container->GetRendererName());
	}

	virtual void Render(float ts) override
	{
		//m_Renderer.RenderCube(m_Camera);
		//m_Renderer.RenderImage(m_Camera);
		m_Renderer[m_CurrentIndex]->Render(m_Camera);
	}

	virtual void ShowUI(float ts) override
	{
		ImGui::Begin("Setting");

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

	virtual void OnUpdate(float ts) override
	{
		m_Camera.OnUpdate(ts);
	}

private:
	std::vector<std::shared_ptr<Renderer>> m_Renderer;
	std::vector<const char*> m_RendererName;
	int m_RendererIndex = 0;
	int m_CurrentIndex = 0;
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