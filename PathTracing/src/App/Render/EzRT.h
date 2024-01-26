#pragma once

#include <vector>

#include "App/Renderer.h"


class EzRT : public Renderer
{
public:
	EzRT();
	virtual void Render(Camera& camera) override;
private:
	glm::vec3 m_Vertices[3];
	unsigned int m_VerticesBuffer;
	unsigned int m_VerticesTexture;
	unsigned int m_VAO;
};