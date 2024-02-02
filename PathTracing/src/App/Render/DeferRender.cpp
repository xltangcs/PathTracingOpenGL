#include "DeferRender.h"

DeferRender::DeferRender()
	:Renderer("Defer Render Test"), 
	geometryShader("./assets/shaders/vshader.vert", "./assets/shaders/g_buffer_frag.glsl"),
	sceneShader   ("./assets/shaders/vshader.vert", "./assets/shaders/scene_frag.glsl"),
	groundGeometry(1.0, 1.0)
{

	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	sceneShader.use();
	sceneShader.setInt("gPosition", 0);


}

void DeferRender::Render(Camera& camera)
{
	glClearColor(25.0 / 255.0, 25.0 / 255.0, 25.0 / 255.0, 1.0);

	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	geometryShader.use();

	glBindVertexArray(groundGeometry.VAO);
	glDrawElements(GL_TRIANGLES, groundGeometry.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);

	glBindVertexArray(groundGeometry.VAO);
	glDrawElements(GL_TRIANGLES, groundGeometry.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


}
 