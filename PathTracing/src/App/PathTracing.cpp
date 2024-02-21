#include "PathTracing.h"

#include <string>
#include <glm/gtc/matrix_transform.hpp>

#include "App/HDRLoader.h"

GLuint getTextureRGB32F(int width, int height) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return tex;
}

PathTracing::PathTracing()
	:
	pass1("./assets/shaders/vshader.vert", "./assets/shaders/fshader.frag"),
	pass2("./assets/shaders/vshader.vert", "./assets/shaders/pass2.frag"),
	pass3("./assets/shaders/vshader.vert", "./assets/shaders/pass3.frag")
{

	//{
	//	// hdr 全景图
	//	HDRLoaderResult hdrRes;
	//	bool r = HDRLoader::load("./assets/textures/HDR/sunset.hdr", hdrRes);
	//	hdrMap = getTextureRGB32F(hdrRes.width, hdrRes.height);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, hdrRes.width, hdrRes.height, 0, GL_RGB, GL_FLOAT, hdrRes.cols);


	//	int BunnyID = m_Scene.AddModel("./assets/models/Stanford Bunny.obj");
	//	Material BunnyMat;
	//	BunnyMat.baseColor = glm::vec3(1, 0, 0);
	//	int BunnyMatId = m_Scene.AddMaterial(BunnyMat);
	//	glm::mat4 BunnyTrans = glm::mat4(1.0f);
	//	BunnyTrans = glm::translate(BunnyTrans, glm::vec3(0.0f, -0.1f, 0.0f)) * glm::scale(BunnyTrans, glm::vec3(4.0f, 4.0f, 4.0f));
	//	ModelInstance Bunny("Bunny", BunnyID, BunnyTrans, BunnyMatId);
	//	m_Scene.AddModelInstance(Bunny);


	//	int quadId = m_Scene.AddModel("./assets/models/quad.obj");
	//	Material blueColor;
	//	blueColor.baseColor = glm::vec3(0.725, 0.71, 0.68);
	//	int blueMatId = m_Scene.AddMaterial(blueColor);
	//	glm::mat4 quadTrans = glm::mat4(1.0f);
	//	quadTrans = glm::translate(quadTrans, glm::vec3(0.0f, -2.0f, 0.0f)) * glm::scale(quadTrans, glm::vec3(20.0f, 0.01f, 20.0f));
	//	ModelInstance quad("Cube", quadId, quadTrans, blueMatId);
	//	m_Scene.AddModelInstance(quad);


	//	int shpereId = m_Scene.AddModel("./assets/models/sphere.obj");
	//	Material whiteColor;
	//	whiteColor.baseColor = glm::vec3(1, 1, 1);
	//	whiteColor.emissive = glm::vec3(30, 20, 10);
	//	int whiteMatId = m_Scene.AddMaterial(whiteColor);
	//	glm::mat4 sphereTrans = glm::mat4(1.0f);
	//	sphereTrans = glm::translate(sphereTrans, glm::vec3(0.0, 0.9, 0.0)) * glm::scale(sphereTrans, glm::vec3(0.1, 0.1, 0.1));
	//	ModelInstance sphere("Sphere", shpereId, sphereTrans, whiteMatId);
	//	m_Scene.AddModelInstance(sphere);

	//	m_Scene.ProcessScene(m_TriangleEncodeds);
	//}

}

void PathTracing::Render(Camera& camera, Scene& scene)
{
	if (isNewScene)
	{
		ProcessData(scene.m_TriangleEncoded);
		CreateFrameBuffer();
	}
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
	glBindTexture(GL_TEXTURE_2D, hdrMap);
	pass1.GetShader().setInt("hdrMap", 3);

	pass1.GetShader().setInt("frameCounter", m_frameIndex ++);
	pass1.GetShader().setInt("cameraMoved", camera.isCameraMoved);
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

void PathTracing::ProcessData(std::vector<TriangleEncoded> triangleEncodeds)
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

void PathTracing::CreateFrameBuffer()
{
	pass1.CreateFrameBuffer(1);
	pass2.CreateFrameBuffer(1);
	pass3.CreateFrameBuffer(0);
}
