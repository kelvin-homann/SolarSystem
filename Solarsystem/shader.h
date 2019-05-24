#ifndef  _SHADER_
#define _SHADER_

#include <GL/glew.h>
#include <iostream>

const GLchar* vShaderCode =
"#version 330 core\n"
"layout(location= 0) in vec3 position;\n"
"uniform mat4 modelviewprojection;\n"
"void main()\n"
"{\n"
"gl_Position = ????? \n"
"}\0";

const GLchar* vShaderCode =
"#version 330 core\n"
"layout(location= 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"uniform mat4 modelviewprojection;\n"
"out vec3 vColor;\n"
"void main()\n"
"{\n"
"gl_Position = modelviewprojection * vec4(position.x, position.y, position.z, 1.0);\n"
"vColor= color;\n"
"}\0";

const GLchar* fShaderCode =
"#version 330 core\n"
"in vec3 vColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
//"color= vec4(vColor, 1.0f);\n"
"color= vec4(1.0, 0.0, 0.0, 1.0f);\n"
"}\n\0";

GLuint shaderProgram;

void initShader(GLuint &vertex, GLuint &fragment)
{
	//GLuint vertex;
	//GLuint fragment;
	GLchar infoLog[512];
	GLint success;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertex);
	glAttachShader(shaderProgram, fragment);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

#endif // ! _SHADER_

