#include "model.h"
#include "imgui.h"
#include "soil.h"

void Model::loadTexture(GLchar* path)
{
    //Generate texture ID and load texture data 
    glGenTextures(1, &m_textureID);
    m_imgData = SOIL_load_image(path, &m_texWidth, &m_texHeight, 0, SOIL_LOAD_RGB);

    // Assign texture to ID
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_texWidth, m_texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_imgData);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(m_imgData);
}

void Model::SetShader(const char* vertexPath, const char* fragmentPath)
{
    shader = Shader(vertexPath, fragmentPath);
}
