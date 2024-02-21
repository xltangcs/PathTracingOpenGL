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
	virtual void OnResize(unsigned int width, unsigned int height) override;

	GLuint CreatTextureBuffer(int size, const void* data);
	void ProcessData();
	void CreateFrameBuffer();


private:
	GLuint m_TrianglesTexture;
	GLuint m_BVHNodesTexture;
	GLuint hdrMap;

	std::vector<TriangleEncoded> m_TriangleEncodeds;
	std::vector<BVHNodeEncoded> m_BVHNodeEncodeds;

	int m_frameIndex = 0;
	RenderPass pass1, pass2, pass3;
	
	Scene m_Scene;
};