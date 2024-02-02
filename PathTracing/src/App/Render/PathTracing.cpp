#include "PathTracing.h"

#include <string>

extern bool setBVH;

GLuint getTextureRGB32F(int width, int height) 
{
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return tex;
}

PathTracing::PathTracing()
	:Renderer("Path Tracing"), m_Plane(2.0, 2.0),
	fbo1(1280, 720), fbo2(1280, 720),
	pathTracingShader("./assets/shaders/vshader.vert", "./assets/shaders/fshader.frag"),
	saveLastShader("./assets/shaders/vshader.vert", "./assets/shaders/pass2.frag"),
	finalShader("./assets/shaders/vshader.vert", "./assets/shaders/pass3.frag")

	//geometryShader("./assets/shaders/vshader.vert", "./assets/shaders/g_buffer_frag.glsl"),
	//sceneShader("./assets/shaders/vshader.vert", "./assets/shaders/scene_frag.glsl")
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

	pathTracingShader.use();
	pathTracingShader.setInt("nTriangles", nTriangles);
	pathTracingShader.setInt("nNodes", nBVHNodes);

	std::cout << "Triangles : " << nTriangles << std::endl;
	std::cout << "BVHNodes : " << nBVHNodes << std::endl;

}

void PathTracing::Render(Camera& camera)
{
	//std::cout <<" frame index :" << m_frameIndex++ << std::endl;

	/****************************************************************/
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	fbo1.Bind();
	glClear(GL_COLOR_BUFFER_BIT);

	pathTracingShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, m_TrianglesTexture);
	pathTracingShader.setInt("TrianglesTexture", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_BUFFER, m_BVHNodesTexture);
	pathTracingShader.setInt("BVHNodesTexture", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_BUFFER, fbo2.GetTextureID());
	pathTracingShader.setInt("lastFrame", 2);

	pathTracingShader.setInt("setBVH", setBVH);
	pathTracingShader.setInt("frameCounter", m_frameIndex ++);
	pathTracingShader.setInt("width", m_Width);
	pathTracingShader.setInt("height", m_Height);
	pathTracingShader.setVec3("cameraPosition", camera.GetPosition());

	glBindVertexArray(m_Plane.VAO);
	glDrawElements(GL_TRIANGLES, m_Plane.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


	/****************************************************************/

	fbo2.Bind();
	glClear(GL_COLOR_BUFFER_BIT);

	saveLastShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo1.GetTextureID());
	saveLastShader.setInt("lastColor", 0);

	glBindVertexArray(m_Plane.VAO);
	glDrawElements(GL_TRIANGLES, m_Plane.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	/****************************************************************/

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	finalShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo2.GetTextureID());
	finalShader.setInt("finalColor", 0);

	glBindVertexArray(m_Plane.VAO);
	glDrawElements(GL_TRIANGLES, m_Plane.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
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
