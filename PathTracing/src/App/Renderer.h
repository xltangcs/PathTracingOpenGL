#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Image.h"
#include "Core/Shader.h"

#include "App/Scene.h"
#include "App/Camera.h""

class Renderer {
public:
	Renderer(char* name = "Renderer") : m_RendererName(name) {};
	virtual ~Renderer() = default;

	virtual void Render(Camera& camera, Scene& scene) {};
	virtual void OnResize(unsigned int width, unsigned int height) {};

	virtual char* GetRendererName() { return m_RendererName; }

protected:
	unsigned int m_Width = 1280, m_Height = 720;

	GLuint m_VAO = 0;
	char* m_RendererName;
};