#include "PathTracing.h"

#include <string>
#include <glm/gtc/matrix_transform.hpp>

#include "App/HDRLoader.h"

PathTracing::PathTracing()
	:
	pass1("./assets/shaders/vshader.vert", "./assets/shaders/fshader.frag"),
	pass2("./assets/shaders/vshader.vert", "./assets/shaders/pass2.frag"),
	pass3("./assets/shaders/vshader.vert", "./assets/shaders/pass3.frag")
{
	pass1.CreateFrameBuffer(1);
	pass2.CreateFrameBuffer(1);
	pass3.CreateFrameBuffer(0);
}

void PathTracing::Render(Camera& camera, Scene& scene)
{
	if (isNewScene) ProcessData(scene.m_TriangleEncoded);
	if (camera.isCameraMoved || isNewScene) m_frameIndex = 0;

	pass1.GetShader().use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, m_TrianglesTexture);
	pass1.GetShader().setInt("triangles", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_BUFFER, m_BVHNodesTexture);
	pass1.GetShader().setInt("nodes", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, pass2.colorAttachments[0]);
	pass1.GetShader().setInt("lastFrame", 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, scene.envMapID);
	pass1.GetShader().setInt("hdrMap", 3);

	pass1.GetShader().setInt("frameCounter", m_frameIndex ++);
	pass1.GetShader().setInt("Reset", camera.isCameraMoved || isNewScene);
	pass1.GetShader().setInt("width", m_Width);
	pass1.GetShader().setInt("height", m_Height);
	pass1.GetShader().setVec3("eye", camera.GetPosition());
	pass1.GetShader().setMat4("cameraRotate", camera.GetInverseView());

	pass1.Draw();
	pass2.Draw(pass1.colorAttachments);
	pass3.Draw(pass2.colorAttachments);
}

void PathTracing::OnResize(unsigned int width, unsigned int height)
{
	if (m_Width != width || m_Height != height)
	{
		m_Width = width;
		m_Height = height;

		//std::cout << " PathTracing " << m_Width << " " << m_Height << "\n";

		pass1.OnResize(m_Width, m_Height);
		pass2.OnResize(m_Width, m_Height);
		pass3.OnResize(m_Width, m_Height);
	}
}

GLuint PathTracing::CreatTextureBuffer(int size, const void* data)
{
	//creat texture storage Triangles data
	GLuint tbo, textureid;
	glGenBuffers(1, &tbo);
	glBindBuffer(GL_TEXTURE_BUFFER, tbo);
	glBufferData(GL_TEXTURE_BUFFER, size, data, GL_STATIC_DRAW);
	glGenTextures(1, &textureid);
	glBindTexture(GL_TEXTURE_BUFFER, textureid);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, tbo);

	return textureid;
}

void PathTracing::ProcessData(std::vector<TriangleEncoded>& triangleEncodeds)
{
	std::vector<BVHNodeEncoded> m_BVHNodeEncodeds;

	int nTriangles = triangleEncodeds.size();

	BVH bvh;
	//bvh.buildBVH(m_TriangleEncodeds, 0, nTriangles - 1);
	bvh.buildBVHwithSAH(triangleEncodeds, 0, nTriangles - 1);

	bvh.translateBVHNodes(m_BVHNodeEncodeds);
	int nBVHNodes = m_BVHNodeEncodeds.size();

	//bvh.printBVHNodeEncodeed(m_BVHNodeEncodeds);

	m_TrianglesTexture = CreatTextureBuffer(nTriangles * sizeof(TriangleEncoded), &triangleEncodeds[0]);
	m_BVHNodesTexture = CreatTextureBuffer(nBVHNodes * sizeof(BVHNodeEncoded), &m_BVHNodeEncodeds[0]);

	pass1.GetShader().use();
	pass1.GetShader().setInt("nTriangles", nTriangles);
	pass1.GetShader().setInt("nNodes", nBVHNodes);

	std::cout << "Triangles : " << nTriangles << std::endl;
	std::cout << "BVHNodes : " << nBVHNodes << std::endl;
}
