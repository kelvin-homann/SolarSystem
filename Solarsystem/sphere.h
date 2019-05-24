#ifndef _SPHERE_H
#define _SPHERE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

#define VERTICES_COUNT_OF_SPHERE 32*32
#define INDICE_COUNT_OF_SPHERE 32*32*6 

class Sphere {
	public:
		Sphere(GLfloat* vertices, GLfloat* normals, GLfloat* texcoords, GLuint* indices, float radius);  // constructor
		//~Sphere();
		float getRadius() const { return _radius; }
		void createSphere(GLfloat* vertices, GLfloat* normals, GLfloat* texcoords, GLuint* indices, float radius);

	private:
		// Variablen 
		GLfloat* _vertices;
		GLfloat* _normals;
		GLfloat* _texcoords;
		GLuint* _indices;
		float _radius;
		const int rings = 16;
		const int sectors = 16;

		inline void push_indices(GLuint* indices, int index, int sectors, int r, int s);
};

#endif // _SPHERE_H
