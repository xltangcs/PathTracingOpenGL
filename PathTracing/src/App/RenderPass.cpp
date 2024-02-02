#include "RenderPass.h"

RenderPass::RenderPass(const char* vertexPath, const char* fragmentPath)
    : m_Shader(vertexPath, fragmentPath)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    std::vector<glm::vec3> square = {
        glm::vec3(-1, -1, 0),
        glm::vec3(1, -1, 0),
        glm::vec3(-1,  1, 0),
        glm::vec3(1,  1, 0),
        glm::vec3(-1,  1, 0),
        glm::vec3(1, -1, 0)
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * square.size(), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * square.size(), &square[0]);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);   // layout (location = 0) 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
}

void RenderPass::CreateFrameBuffer(int attachmentNum)
{
    if (attachmentNum == 0) return;

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    //// 不是 finalPass 则生成帧缓冲的颜色附件
    //if (!finalPass) {
    //    std::vector<GLuint> attachments;
    //    for (int i = 0; i < colorAttachments.size(); i++) {
    //        glBindTexture(GL_TEXTURE_2D, colorAttachments[i]);
    //        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorAttachments[i], 0);// 将颜色纹理绑定到 i 号颜色附件
    //        attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    //    }
    //    glDrawBuffers(attachments.size(), &attachments[0]);
    //}

    for (size_t i = 0; i < attachmentNum; i++)
    {
        // create a color attachment texture
        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_TextureID, 0);

        colorAttachments.push_back(m_TextureID);
    }
    glDrawBuffers(attachmentNum, &colorAttachments[0]);

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        //std::cout << glCheckFramebufferStatus(GL_FRAMEBUFFER);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass::Draw(std::vector<GLuint> texPassArray)
{
    m_Shader.use();
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glBindVertexArray(vao);
    // 传上一帧的帧缓冲颜色附件
    for (int i = 0; i < texPassArray.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texPassArray[i]);
        std::string uName = "texPass" + std::to_string(i);
        //m_Shader.setInt(uName.c_str(), i);
        glUniform1i(glGetUniformLocation(m_Shader.GetShaderID(), uName.c_str()), i);
    }
    glViewport(0, 0, m_Width, m_Height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(0);
}

void RenderPass::AddColorAttachment(GLuint tattachment)
{
    colorAttachments.push_back(tattachment);
}
