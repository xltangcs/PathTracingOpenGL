//#include "PathTracing.h"
//
//extern bool setBVH;
//
//PathTracing::PathTracing()
//	:Renderer("Path Tracing"), m_Plane(2.0, 2.0),
//	m_PlaneShader("./assets/shaders/Base.vert", "./assets/shaders/Base.frag"),
//	pass1Shader("./assets/shaders/vshader.vert", "./assets/shaders/fshader.frag")
//{
//	//m_Scene.AddModel("./assets/models/Stanford Bunny.obj");
//	for (size_t i = 0; i < 50; i++)
//	{
//		m_Scene.AddModel("./assets/models/quad.obj");
//	}
//	//m_Scene.AddModel("./assets/models/quad.obj");
//	//m_Scene.AddModel("./assets/models/sphere.obj");
//
//	int nTriangles = m_Scene.GetTriangleNum();
//	BVH bvh;
//	BVHNode* bvhNode = bvh.buildBVH(m_Scene.GetTriangles(), 0, nTriangles - 1);
//	bvh.flattenBVHTree(bvhNode);
//
//	//bvh.printBVHNodeEncodeed();
//
//	int nBVHNodes = bvh.GetBVHNodeNum();
//
//	m_TrianglesTexture = CreatTextureBuffer(nTriangles * sizeof(TriangleEncoded), m_Scene.GetTriangleAddr());
//	m_BVHNodesTexture = CreatTextureBuffer(nBVHNodes * sizeof(BVHNodeEncoded), bvh.GetBVHNodeEncodedAddr());
//
//	pass1Shader.use();
//	pass1Shader.setInt("nTriangles", nTriangles);
//	pass1Shader.setInt("nNodes", nBVHNodes);
//
//	std::cout << "Triangles : " << nTriangles << std::endl;
//	std::cout << "BVHNodes : " << nBVHNodes << std::endl;
//}
//
//void PathTracing::Render(Camera& camera)
//{
//	//std::cout <<" frame index :" << m_frameIndex++ << std::endl;
//	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_TEST);
//
//	pass1Shader.use();
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_BUFFER, m_TrianglesTexture);
//	pass1Shader.setInt("TrianglesTexture", 0);
//
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_BUFFER, m_BVHNodesTexture);
//	pass1Shader.setInt("BVHNodesTexture", 1);
//	pass1Shader.setInt("setBVH", setBVH);
//	pass1Shader.setVec3("cameraPosition", camera.GetPosition());
//
//	glBindVertexArray(m_Plane.VAO);
//	glDrawElements(GL_TRIANGLES, m_Plane.indices.size(), GL_UNSIGNED_INT, 0);
//}
//
//GLuint PathTracing::CreatTextureBuffer(int size, const void* data)
//{
//	//creat texture storage Triangles data
//	GLuint tbo, textureid;
//	glGenBuffers(1, &tbo);
//	glBindBuffer(GL_TEXTURE_BUFFER, tbo);
//	glBufferData(GL_TEXTURE_BUFFER, size, data, GL_STATIC_DRAW);
//	glGenTextures(1, &textureid);
//	glBindTexture(GL_TEXTURE_BUFFER, textureid);
//	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, tbo);
//
//	return textureid;
//}
