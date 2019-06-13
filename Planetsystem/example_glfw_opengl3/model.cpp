#include "model.h"
#include "imgui.h"
#include "soil.h"
#include <iostream>

void Model::loadTexture(GLchar* path)
{
    //Generate texture ID and load texture data 
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    m_imgData = SOIL_load_image(path, &m_texWidth, &m_texHeight, 0, SOIL_LOAD_RGB);
    if (m_imgData)
    {
        // Assign texture to ID
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_texWidth, m_texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_imgData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(m_imgData);
}

void Model::SetShader(const char* vertexPath, const char* fragmentPath)
{
    shader = Shader(vertexPath, fragmentPath);
}
