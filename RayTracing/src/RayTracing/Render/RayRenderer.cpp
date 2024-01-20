#include "RayRenderer.h"

#include "Core/Geometry/PlaneGeometry.h"

RayRenderer::RayRenderer()
	:Renderer("Ray Renderer")
{

}

void RayRenderer::Render(Camera& camera)
{
	auto m_Shader = std::make_shared<Shader>("./assets/shaders/RayTracing.vs", "./assets/shaders/RayTracing.fs");
	PlaneGeometry m_Plane(2.0f, 2.0f);

    //begin render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    int width = camera.GetWidth();
    int height = camera.GetHeight();
    float FOV = (float)width / (float)height;
    float viewportHeight = 2.0f;
    float viewportWidht = viewportHeight * FOV;

    auto cameraPosition = camera.GetPosition();
    // activate shader
    m_Shader->use();
    m_Shader->setVec3("camera.lower_left_corner", cameraPosition - glm::vec3(viewportWidht / 2.0f, viewportHeight / 2.0f, 1.0f));
    m_Shader->setVec3("camera.horizontal", glm::vec3(viewportWidht, 0.0f, 0.0f));
    m_Shader->setVec3("camera.vertical", glm::vec3(0.0f, viewportHeight, 0.0f));
    m_Shader->setVec3("camera.position", cameraPosition);

    glBindVertexArray(m_Plane.VAO);
    glDrawElements(GL_TRIANGLES, m_Plane.indices.size(), GL_UNSIGNED_INT, 0);

}
