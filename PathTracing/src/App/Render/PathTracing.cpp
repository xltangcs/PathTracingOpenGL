#include "PathTracing.h"


PathTracing::PathTracing()
	:Renderer("Path Tracing")
{
    {
        m_Vertices[0] = glm::vec3(1.0f, 0.0f, 0.0f);
        m_Vertices[1] = glm::vec3(0.0f, 1.0f, 0.0f);
        m_Vertices[2] = glm::vec3(0.0f, 0.0f, 1.0f);

        GLuint tbo;
        glGenBuffers(1, &tbo);
        glBindBuffer(GL_TEXTURE_BUFFER, tbo);
        glBufferData(GL_TEXTURE_BUFFER, sizeof(m_Vertices), m_Vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_TEXTURE_BUFFER, 0);

        glGenTextures(1, &m_VerticesTexture);
        glBindTexture(GL_TEXTURE_BUFFER, m_VerticesTexture);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, tbo);
        glBindTexture(GL_TEXTURE_BUFFER, 0);
    }

    {
        float vertices[] = {
             1.0f,  1.0f, 0.0f,  // top right
             1.0f, -1.0f, 0.0f,  // bottom right
            -1.0f, -1.0f, 0.0f,  // bottom left
            -1.0f,  1.0f, 0.0f   // top left 
        };
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
        };
        unsigned int VBO, EBO;
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void PathTracing::Render(Camera& camera)
{
    Shader m_PassShader("./assets/shaders/PassData.vs", "./assets/shaders/PassData.fs");
    m_PassShader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_BUFFER, m_VerticesTexture);
    m_PassShader.setInt("TexturePassData", 0);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
