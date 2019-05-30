#ifndef _CREATE_SHADER_H
#define _CREATE_SHADER_H
#include <GL/gl3w.h> 

class Shader
{
public:
    Shader();
    Shader(const char* vertexPath, const char* fragmentPath);
    void PrintLog(GLuint object);
    GLuint CreateGsProgram(const char* vertexfile, const char* geometryfile, const char* fragmentfile, GLint input, GLint output, GLint vertices);
    GLint GetAttrib(GLuint program, const char* name);
    GLint GetUniform(GLuint program, const char* name);
    GLint GetVertex() const { return vertex_shader; }
    GLint GetFragment() const { return fragment_shader; }
    GLint GetShaderProgram() const { return shader_program; }
    void Use();
private:
    GLuint shader_program = 0;
    GLuint vertex_shader = 0;
    GLuint fragment_shader = 0;
    GLuint CreateProgram(const char* vertexfile, const char* fragmentfile);
    GLuint CreateShader(const char* filename, GLenum type);
    char* FileRead(const char* filename);
};

#endif
