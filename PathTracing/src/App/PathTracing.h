#pragma once

#include "Core/Framebuffer.h"

#include "App/Renderer.h"
#include "App/Triangle.h"
#include "App/Material.h"
#include "App/BVH.h"
#include "App/RenderPass.h"

class PathTracing
{
public:
	bool isNewScene = true;
public:
	PathTracing();

	void Render(Camera& camera, Scene& scene);
	void OnResize(unsigned int width, unsigned int height);

	GLuint CreatTextureBuffer(int size, const void* data);
	void ProcessData(std::vector<TriangleEncoded> triangleEncodeds);
	void CreateFrameBuffer();


private:
	unsigned int m_Width = 1280, m_Height = 720;

	GLuint m_TrianglesTexture;
	GLuint m_BVHNodesTexture;
	GLuint hdrMap;

	//std::vector<TriangleEncoded> m_TriangleEncodeds;
	//std::vector<BVHNodeEncoded> m_BVHNodeEncodeds;

	int m_frameIndex = 0;
	RenderPass pass1, pass2, pass3;
	GLuint m_VAO = 0;

};