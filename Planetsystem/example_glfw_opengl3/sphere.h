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
		~Sphere();

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

        int GetVBO() const { return _vbo_vertices; }
        int GetIBO() const { return _ibo; }

        void SetColor(glm::vec4 color);
        void BindBuffers();

        void BindShader();
        void Render();

        float m_rotSpeed = 0;
        float m_distanceToParent = 0;

	private:
		// Variablen
		float _radius = 0.0f;
        glm::vec4 _color = glm::vec4(1);
		const long rings = 16;
		const long sectors = 16;

        GLfloat vertices_[VERTICES_COUNT_OF_SPHERE * 3];
        GLfloat normals_[VERTICES_COUNT_OF_SPHERE * 3];
        GLfloat texcoords_[VERTICES_COUNT_OF_SPHERE * 2];
        GLuint indices_[INDICE_COUNT_OF_SPHERE];

        size_t vertices_size = sizeof(vertices_);
        size_t normals_size = sizeof(normals_);
        size_t texcoords_size = sizeof(texcoords_);
        size_t indices_size = sizeof(indices_);

        GLuint _ibo = 0;
        GLuint _vbo_vertices = 0;
        GLuint _vbo_normals = 0;
        GLuint _vbo_textures = 0;

		void CreateSphere(GLfloat* vertices, GLfloat* normals, GLfloat* texcoords, GLuint* indices, float radius);

		inline void push_indices(GLuint* indices, int index, int sectors, int r, int s);
};

#endif // _SPHERE_H
