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

	virtual void OnResize(unsigned int width, unsigned int height)
	{
		if(width != m_width) m_width = width;
		if(height != m_height) m_height = height;
	}

	virtual void Render(Camera& camera) {};
	virtual char* GetRendererName() { return m_RendererName; }

protected:
	unsigned int m_width = 0, m_height = 0;

	GLuint m_VAO = 0;
	char* m_RendererName;
};