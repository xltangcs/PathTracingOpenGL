#include "PBR.h"

#include "Core/Geometry/SphereGeometry.h"

PBR::PBR()
	:Renderer("PBR Test")
{
}

void PBR::Render(Camera& camera)
{
	SphereGeometry m_Sphere(1.0f);
	auto m_Shader = std::make_shared<Shader>("./assets/shaders/Base.vs", "./assets/shaders/RedColor.fs");
    
    //begin render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // activate shader
    m_Shader->use();
    m_Shader->setMat4("model",glm::mat4(1.0f));
    m_Shader->setMat4("view",camera.GetView());
    m_Shader->setMat4("projection", camera.GetProjection());

    glBindVertexArray(m_Sphere.VAO);
    glDrawElements(GL_TRIANGLES, m_Sphere.indices.size(), GL_UNSIGNED_INT, 0);
}
