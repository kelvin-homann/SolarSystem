#ifndef _SPHERE_H
#define _SPHERE_H

#include <GL/gl3w.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include "model.h"

#define VERTICES_COUNT_OF_SPHERE 32*32
#define INDICE_COUNT_OF_SPHERE 32*32*6 

class Sphere : public Model {
	public:
		Sphere(float radius);  // constructor
		//~Sphere(); // destructor
		float GetRadius() const { return _radius; }
        glm::vec4 GetColor() const { return _color; }
		int GetVerticesSize() const { return vertices_size; }
		int GetNormalsSize() const { return normals_size; }
		int GetTexcoordsSize() const { return texcoords_size; }
		int GetIndicesSize() const { return indices_size; }
        GLfloat* GetVertices() { return vertices_; }
        GLfloat* GetNormals() { return normals_; }
        GLfloat* GetTexcoords() { return texcoords_; }
        GLuint* GetIndices() { return indices_; }
        void SetColor(glm::vec4 color);
        void BindBuffers(GLuint *vbo, GLuint *ibo);

	private:
		// Variablen 
		float _radius;
        glm::vec4 _color;
		const int rings = 16;
		const int sectors = 16;

        GLfloat vertices_[VERTICES_COUNT_OF_SPHERE];
        GLfloat normals_[VERTICES_COUNT_OF_SPHERE];
        GLfloat texcoords_[VERTICES_COUNT_OF_SPHERE];
        GLuint indices_[INDICE_COUNT_OF_SPHERE];

        int vertices_size = sizeof(vertices_);
        int normals_size = sizeof(normals_);
        int texcoords_size = sizeof(texcoords_);
        int indices_size = sizeof(indices_);

        //GLuint vbo_sphere_vertices;
        //GLuint ibo_sphere_elements;
		void CreateSphere(GLfloat* vertices, GLfloat* normals, GLfloat* texcoords, GLuint* indices, float radius);

		inline void push_indices(GLuint* indices, int index, int sectors, int r, int s);
};

#endif // _SPHERE_H
