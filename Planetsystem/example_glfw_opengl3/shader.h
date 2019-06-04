#ifndef _CREATE_SHADER_H
#define _CREATE_SHADER_H
#include <GL/gl3w.h>
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader();
    Shader(const char* vertexPath, const char* fragmentPath);
    GLint GetVertex() const { return vertex_shader; }
    GLint GetFragment() const { return fragment_shader; }
    GLint GetShaderProgram() const { return shader_program; }
    void Use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w);
    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    GLuint shader_program = 0;
    GLuint vertex_shader = 0;
    GLuint fragment_shader = 0;
    void checkCompileErrors(GLuint shader, std::string type);   
};

#endif
