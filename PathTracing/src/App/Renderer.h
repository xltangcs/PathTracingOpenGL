#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Image.h"
#include "Core/Shader.h"

#include <App/Camera.h>

class Renderer {
public:
	Renderer(char* name = "Renderer") : m_RendererName(name) {};
	virtual ~Renderer() = default;

	virtual void Render(Camera& camera) {};
	virtual char* GetRendererName() { return m_RendererName; }

protected:
	GLuint m_VAO = 0;
	char* m_RendererName;
};