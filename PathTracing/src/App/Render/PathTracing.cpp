#include "PathTracing.h"

#include "App/Model.h"


PathTracing::PathTracing()
	:Renderer("Path Tracing"), m_Plane(2.0, 2.0),
	m_PlaneShader("./assets/shaders/Base.vert", "./assets/shaders/Base.frag"),
	pass1Shader("./assets/shaders/vshader.vert", "./assets/shaders/fshader.frag")
{
	Model cbox_redwall("./assets/models/Stanford Bunny.obj");
	//Model cbox_redwall("./assets/models/Quad.obj");
	//Model cbox_redwall("./assets/models/cornell_box/cbox_redwall.obj");
	//m_Triangle.push_back(cbox_redwall.modelTriangles);
	//Model cbox_redwall("./assets/models/cornell_box/cbox_greenwall.obj");

	int nTriangles = cbox_redwall.modelTriangles.size();
	pass1Shader.use();
	pass1Shader.setInt("nTriangles", nTriangles);

	std::vector<Triangle_encoded> triangles_encoded(nTriangles);

	for (int i = 0; i < nTriangles; i++)
	{
		auto t = cbox_redwall.modelTriangles[i];
		Material m;
		//m.baseColor = glm::vec3(0.725, 0.71, 0.68);
		m.baseColor = glm::vec3(1.0, 0.0, 0.0);

		triangles_encoded[i].p1 = t.position[0];
		triangles_encoded[i].p2 = t.position[1];
		triangles_encoded[i].p3 = t.position[2];

		triangles_encoded[i].n3 = t.normal[0];
		triangles_encoded[i].n3 = t.normal[1];
		triangles_encoded[i].n3 = t.normal[2];

		// ²ÄÖÊ
		triangles_encoded[i].emissive = m.emissive;
		triangles_encoded[i].baseColor = m.baseColor;
		triangles_encoded[i].param1 = glm::vec3(m.subsurface, m.metallic, m.specular);
		triangles_encoded[i].param2 = glm::vec3(m.specularTint, m.roughness, m.anisotropic);
		triangles_encoded[i].param3 = glm::vec3(m.sheen, m.sheenTint, m.clearcoat);
		triangles_encoded[i].param4 = glm::vec3(m.clearcoatGloss, m.IOR, m.transmission);
	}

	//creat texture storage Triangles data
	GLuint tbo0;
	glGenBuffers(1, &tbo0);
	glBindBuffer(GL_TEXTURE_BUFFER, tbo0);
	glBufferData(GL_TEXTURE_BUFFER, triangles_encoded.size() * sizeof(Triangle_encoded), &triangles_encoded[0], GL_STATIC_DRAW);
	glGenTextures(1, &m_TrianglesTextureBuffer);
	glBindTexture(GL_TEXTURE_BUFFER, m_TrianglesTextureBuffer);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, tbo0);

	//Image image1()

}

void PathTracing::Render(Camera& camera)
{
	std::cout <<" frame index :" << m_frameIndex++ << std::endl;
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	pass1Shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, m_TrianglesTextureBuffer);
	pass1Shader.setInt("triangles", 0);


	glBindVertexArray(m_Plane.VAO);
	glDrawElements(GL_TRIANGLES, m_Plane.indices.size(), GL_UNSIGNED_INT, 0);
}
