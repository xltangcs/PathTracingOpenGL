#pragma once
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Core/Image.h>
#include <Core/Shader.h>
#include <RayTracing/Camera.h>

class Renderer {
public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render();

	void RenderImage(Camera& camera);
	void RenderCube(Camera& camera);

private:
	GLuint m_CubeVA = 0;
	GLuint m_ImageVA = 0;
};