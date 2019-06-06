#ifndef _MODEL_H
#define _MODEL_H

#include <GL/gl3w.h> 
#include <glm/glm.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include "shader.h"
#include "material.h"

class Model {
public:
    void SetShader(const char* vertexPath, const char* fragmentPath);
    glm::mat4 GetMVP() const { return _mvp; }
    Shader GetShader() const { return shader; }

    glm::vec3 position = glm::vec3(0.0, 0.0, 0.0f);
    glm::vec3 rotation = glm::vec3(0, 1, 0);

    // Texture
    GLuint m_textureID;
    unsigned char* m_imgData;
    int m_texWidth;
    int m_texHeight;

    void loadTexture(GLchar* path);

    // Material
    Material material = Material();
private:
    // Modelviewprojection
    glm::mat4 _model = glm::mat4(1);
    glm::mat4 _view = glm::mat4(1);
    glm::mat4 _projection = glm::mat4(1);
    glm::mat4 _mvp = glm::mat4(1);

    // Shader
    Shader shader = Shader();
};

#endif
