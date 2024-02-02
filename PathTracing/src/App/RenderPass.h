#pragma once

#include <vector>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Core/Shader.h"

class RenderPass {
public:
    RenderPass() = default;
    RenderPass(const char* vertexPath, const char* fragmentPath);

    void CreateFrameBuffer(int attachmentNum);
    void Draw(std::vector<GLuint> texPassArray = {});
    void AddColorAttachment(GLuint tattachment); 


    std::vector<GLuint>& GetColorAttachments() { return colorAttachments; }
    Shader& GetShader() { return m_Shader; }
    GLuint& GetFBO() { return FBO; }
    GLuint& GetTextureID() { return m_TextureID; }
    GLuint& GetColorAttachmentID(int id) { return colorAttachments[id]; } //array bound

private:
    unsigned int m_Width = 1280, m_Height = 720;
    Shader m_Shader;
    GLuint vao;
    GLuint FBO = 0;
    GLuint m_TextureID;

    std::vector<GLuint> colorAttachments;

};