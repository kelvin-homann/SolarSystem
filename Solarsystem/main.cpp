#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/freeglut.h>
/* GLM */
// #define GLM_MESSAGES
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_utils.h"
#include "sphere.h"
#include <iostream>

int screen_width = 800, screen_height = 600;
float cameraDistance = 4.0f;
int drawMode;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;

GLuint vbo_sphere_vertices, vbo_cube_colors;
GLuint ibo_sphere_elements;
GLuint program;
GLint attribute_coord3d, attribute_v_color;
GLint uniform_mvp;

GLfloat sphere_vertices[VERTICES_COUNT_OF_SPHERE];
GLfloat sphere_normals[VERTICES_COUNT_OF_SPHERE];
GLfloat sphere_texcoords[VERTICES_COUNT_OF_SPHERE];
GLuint sphere_indices[INDICE_COUNT_OF_SPHERE];

float sphereRadius = 1.0f;

float rotSpeed = 1000.0;

glm::vec3 axis(0, 1, 0); // default Angle
float elapsedTime;
float tempTime;

void mouseCB(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;

	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			std::cout << mouseRightDown << "\n";
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}
}


void mouseMotionCB(int x, int y)
{
	if (mouseRightDown)
	{
		cameraDistance -= (y - mouseY) * 0.2f;
		mouseY = y;		
	}
}

bool initSharedMem()
{
	mouseRightDown = false;
	mouseX = mouseY = 0;

	cameraAngleX = cameraAngleY = 0.0f;
	drawMode = 0; // 0:fill, 1: wireframe, 2:points

	return true;
}

int init_resources()
{
	// Creating Sphere Data
	Sphere sphere1(sphere_vertices, sphere_normals, sphere_texcoords, sphere_indices, sphereRadius);
	//Sphere sphere2(sphere_vertices, sphere_normals, sphere_texcoords, sphere_indices, 3);

	glGenBuffers(1, &vbo_sphere_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

	GLfloat cube_colors[] = {
		// front colors
		0.5, 0.0, 0.0,
		0.0, 0.5, 0.0,
		0.0, 0.0, 1.0,
		0.5, 0.0, 1.0,
		// back colors
		0.5, 0.0, 0.0,
		0.0, 0.5, 0.0,
		0.0, 0.0, 1.0,
		0.5, 0.5, 0.0,
	};
	glGenBuffers(1, &vbo_cube_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo_sphere_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_sphere_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_indices), sphere_indices, GL_STATIC_DRAW);

	GLint link_ok = GL_FALSE;

	GLuint vs, fs;
	if ((vs = create_shader("cube.v.glsl", GL_VERTEX_SHADER)) == 0) return 0;
	if ((fs = create_shader("cube.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		fprintf(stderr, "glLinkProgram:");
		print_log(program);
		return 0;
	}

	const char* attribute_name;
	attribute_name = "coord3d";
	attribute_coord3d = glGetAttribLocation(program, attribute_name);
	if (attribute_coord3d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}
	attribute_name = "v_color";
	attribute_v_color = glGetAttribLocation(program, attribute_name);
	if (attribute_v_color == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}
	const char* uniform_name;
	uniform_name = "mvp";
	uniform_mvp = glGetUniformLocation(program, uniform_name);
	if (uniform_mvp == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	return 1;
}

void onIdle() {
	initSharedMem();
	elapsedTime = glutGet(GLUT_ELAPSED_TIME);
	float angle = elapsedTime / rotSpeed * 20;  // 20° per rot speed
	glm::mat4 anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0f));
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, -cameraDistance), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::perspective(45.0f, 1.0f * screen_width / screen_height, 0.1f, 10.0f);

	glm::mat4 modelviewprojection = projection * view * model * anim;

	//glm::mat4 translatedmvp = modelviewprojection * glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -1.0));
	glUseProgram(program);
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(modelviewprojection));
	glutPostRedisplay();
}

void onDisplay()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	glEnableVertexAttribArray(attribute_coord3d);
	// Describe our vertices array to OpenGL (it can't guess its format automatically)
	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_vertices);
	glVertexAttribPointer(
		attribute_coord3d, // attribute
		3,                 // number of elements per vertex, here (x,y,z)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);

	glEnableVertexAttribArray(attribute_v_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glVertexAttribPointer(
		attribute_v_color, // attribute
		3,                 // number of elements per vertex, here (R,G,B)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);

	/* Push each element in buffer_vertices to the vertex shader */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_sphere_elements);
	int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(attribute_coord3d);
	glDisableVertexAttribArray(attribute_v_color);
	glutSwapBuffers();
}

void onReshape(int width, int height) {
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, screen_width, screen_height);
}

void keyboardFunction(unsigned char key, int mousex, int mousey)
{
	if (key == '+') {

	}
	else if (key == '-') {

	}
	//if (key == '+') {
	//	std::cout << "Increased speed\n";
	//	rotSpeed *= 0.5;
	//}
	//else if (key == '-') {
	//	std::cout << "Lowered speed\n";
	//	rotSpeed *= 2;
	//}

	//if (key == 'w') {
	//	axis = glm::vec3(0, 1, 0);
	//}
	//else if (key == 's') {
	//	axis = glm::vec3(0, -1, 0);
	//}
	//else if (key == 'd') {
	//	axis = glm::vec3(1, 0, 0);
	//}
	//else if (key == 'a') {
	//	axis = glm::vec3(-1, 0, 0);
	//}
}

void free_resources()
{
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_sphere_vertices);
	glDeleteBuffers(1, &vbo_cube_colors);
	glDeleteBuffers(1, &ibo_sphere_elements);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutCreateWindow("Solarsystem");
	glutKeyboardFunc(keyboardFunction);
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);

	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return 1;
	}

	if (init_resources()) {
		glutDisplayFunc(onDisplay);
		glutReshapeFunc(onReshape);
		glutIdleFunc(onIdle);

		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glDepthFunc(GL_LESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glutMainLoop();
	}
	free_resources();
	return 0;
}
