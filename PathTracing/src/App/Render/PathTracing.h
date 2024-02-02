#pragma once

#include "Core/Geometry/PlaneGeometry.h"
#include "Core/Framebuffer.h"

#include "App/Renderer.h"
#include "App/Triangle.h"
#include "App/Material.h"
#include "App/Scene.h"
#include "App/BVH.h"

class PathTracing : public Renderer
{
public:
	PathTracing();
	virtual void Render(Camera& camera) override;

	GLuint CreatTextureBuffer(int size, const void* data);

private:
	GLuint m_TrianglesTexture;
	GLuint m_BVHNodesTexture;

	PlaneGeometry m_Plane;

	Shader pathTracingShader, saveLastShader, finalShader;

	Scene m_Scene;

	int m_frameIndex = 0;

	Framebuffer fbo1, fbo2;


};