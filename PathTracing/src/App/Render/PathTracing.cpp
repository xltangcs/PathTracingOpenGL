#include "PathTracing.h"

#include <string>

extern bool setBVH;

PathTracing::PathTracing()
	:Renderer("Path Tracing"), m_Plane(2.0, 2.0),
	m_PlaneShader("./assets/shaders/Base.vert", "./assets/shaders/Base.frag"),
	pass1Shader("./assets/shaders/vshader.vert", "./assets/shaders/fshader.frag")
{
	std::vector<TriangleEncoded> m_TriangleEncodeds;
	std::vector<BVHNodeEncoded> m_BVHNodeEncodeds;

	//m_Scene.AddModel("./assets/models/Stanford Bunny.obj");

	int quadId = m_Scene.AddModel("./assets/models/quad.obj");
	Material blueColor;
	blueColor.baseColor = glm::vec3(0.1f, 0.2f, 0.6f);
	//blueColor.emissive = glm::vec3(0.2f, 0.0f, 0.0f);
	int blueMatId = m_Scene.AddMaterial(blueColor);
	glm::mat4 quadTrans = glm::mat4(1.0f);
	quadTrans = glm::scale(quadTrans, glm::vec3(0.5f, 0.5f, 0.5f));
	ModelInstance quad("Cube", quadId, quadTrans, blueMatId);
	m_Scene.AddModelInstance(quad);


	int shpereId = m_Scene.AddModel("./assets/models/sphere.obj");
	Material whiteColor;
	whiteColor.baseColor = glm::vec3(0.9f, 0.9f, 0.9f);
	whiteColor.emissive = glm::vec3(30.0f, 30.0f, 30.0f);
	int whiteMatId = m_Scene.AddMaterial(whiteColor);
	glm::mat4 sphereTrans = glm::mat4(1.0f);
	sphereTrans = glm::translate(sphereTrans, glm::vec3(0.0f, 0.5f, 0.5f)) * glm::scale(sphereTrans, glm::vec3(0.05f, 0.05f, 0.05f));
	ModelInstance sphere("Sphere", shpereId, sphereTrans, whiteMatId);
	m_Scene.AddModelInstance(sphere);


	m_Scene.ProcessScene(m_TriangleEncodeds);

	int nTriangles = m_TriangleEncodeds.size();
	BVH bvh;
	bvh.buildBVH(m_TriangleEncodeds, 0, nTriangles - 1);

	bvh.translateBVHNodes(m_BVHNodeEncodeds);
	int nBVHNodes = m_BVHNodeEncodeds.size();

	//bvh.printBVHNodeEncodeed(m_BVHNodeEncodeds);

	m_TrianglesTexture = CreatTextureBuffer(nTriangles * sizeof(TriangleEncoded), &m_TriangleEncodeds[0]);
	m_BVHNodesTexture = CreatTextureBuffer(nBVHNodes * sizeof(BVHNodeEncoded), &m_BVHNodeEncodeds[0]);

	pass1Shader.use();
	pass1Shader.setInt("nTriangles", nTriangles);
	pass1Shader.setInt("nNodes", nBVHNodes);

	std::cout << "Triangles : " << nTriangles << std::endl;
	std::cout << "BVHNodes : " << nBVHNodes << std::endl;
}

void PathTracing::Render(Camera& camera)
{
	//std::cout <<" frame index :" << m_frameIndex++ << std::endl;
	m_frameIndex++;
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_TEST);

	pass1Shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, m_TrianglesTexture);
	pass1Shader.setInt("TrianglesTexture", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_BUFFER, m_BVHNodesTexture);
	pass1Shader.setInt("BVHNodesTexture", 1);

	pass1Shader.setInt("setBVH", setBVH);
	pass1Shader.setInt("frameCounter", m_frameIndex);
	pass1Shader.setInt("width", m_width);
	pass1Shader.setInt("height", m_height);
	pass1Shader.setVec3("cameraPosition", camera.GetPosition());

	glBindVertexArray(m_Plane.VAO);
	glDrawElements(GL_TRIANGLES, m_Plane.indices.size(), GL_UNSIGNED_INT, 0);
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
