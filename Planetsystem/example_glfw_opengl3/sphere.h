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
		//~Sphere();
		float GetRadius() const { return _radius; }
		int GetVerticesSize() const { return vertices_size; }
		int GetNormalsSize() const { return normals_size; }
		int GetTexcoordsSize() const { return texcoords_size; }
		int GetIndicesSize() const { return indices_size; }
        GLfloat* GetVertices() { return _sphere_vertices; }
        GLfloat* GetNormals() { return _sphere_normals; }
        GLfloat* GetTexcoords() { return _sphere_texcoords; }
        GLuint* GetIndices() { return _sphere_indices; }
		void CreateSphere(GLfloat* vertices, GLfloat* normals, GLfloat* texcoords, GLuint* indices, float radius);
        void SetColor(glm::vec4 color);
        glm::vec4 GetColor() const { return _color; }
        void Init(GLuint *vbo, GLfloat* vertices, GLuint *ibo, GLuint* indices);

	private:
		// Variablen 
		//GLfloat* _vertices;
		//GLfloat* _normals;
		//GLfloat* _texcoords;
		//GLuint* _indices;
		float _radius;
        glm::vec4 _color;
		const int rings = 16;
		const int sectors = 16;

        GLfloat _sphere_vertices[VERTICES_COUNT_OF_SPHERE];
        GLfloat _sphere_normals[VERTICES_COUNT_OF_SPHERE];
        GLfloat _sphere_texcoords[VERTICES_COUNT_OF_SPHERE];
        GLuint _sphere_indices[INDICE_COUNT_OF_SPHERE];

        int vertices_size = sizeof(_sphere_vertices);
        int normals_size = sizeof(_sphere_normals);
        int texcoords_size = sizeof(_sphere_texcoords);
        int indices_size = sizeof(_sphere_indices);

		inline void push_indices(GLuint* indices, int index, int sectors, int r, int s);
};

#endif // _SPHERE_H
