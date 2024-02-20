#pragma once

#include "Core/Framebuffer.h"

#include "App/Renderer.h"
#include "App/Triangle.h"
#include "App/Material.h"
#include "App/Scene.h"
#include "App/BVH.h"
#include "App/RenderPass.h"

class PathTracing : public Renderer
{
public:
	PathTracing();
	virtual void Render(Camera& camera) override;

	GLuint CreatTextureBuffer(int size, const void* data);

private:
	GLuint m_TrianglesTexture;
	GLuint m_BVHNodesTexture;
	GLuint lastFrame;

	GLuint hdrMap;

	Scene m_Scene;
	int m_frameIndex = 0;

	RenderPass pass1, pass2, pass3;
};