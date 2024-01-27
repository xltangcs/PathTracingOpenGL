#pragma once

#include "Core/Geometry/PlaneGeometry.h"

#include "App/Renderer.h"
#include "App/Triangle.h"
#include "App/Material.h"



class PathTracing : public Renderer
{
public:
	PathTracing();
	virtual void Render(Camera& camera) override;
private:
    std::vector<Triangle> m_Triangles;

	GLuint m_TrianglesTextureBuffer;

	PlaneGeometry m_Plane;

	Shader m_PlaneShader;
	Shader pass1Shader;

	int m_frameIndex = 0;
};