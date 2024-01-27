#pragma once

#include "Core/Geometry/PlaneGeometry.h"

#include "App/Renderer.h"
#include "App/Triangle.h"
#include "App/Material.h"
#include "App/Scene.h"



class PathTracing : public Renderer
{
public:
	PathTracing();
	virtual void Render(Camera& camera) override;

	GLuint CreatTextureBuffer(int size, const void* data);

private:
	GLuint m_TrianglesTextureBuffer;

	PlaneGeometry m_Plane;

	Shader m_PlaneShader;
	Shader pass1Shader;

	Scene m_Scene;

	int m_frameIndex = 0;
};