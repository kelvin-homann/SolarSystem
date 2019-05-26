/**
 * From the OpenGL Programming wikibook: http://en.wikibooks.org/wiki/OpenGL_Programming
 * This file is in the public domain.
 * Contributors: Sylvain Beucler
 */
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
    GLint GetShader() const { return shader_program; }
private:
    GLuint shader_program; // Shader Program
    GLuint vertex_shader, fragment_shader;
    GLuint CreateProgram(const char* vertexfile, const char* fragmentfile);
    GLuint CreateShader(const char* filename, GLenum type);
    char* FileRead(const char* filename);
};

#endif
