#include "sphere.h"

// Constructor
Sphere::Sphere(float radius) {
	float _radius = radius;

	CreateSphere(vertices_, normals_, texcoords_, indices_, _radius);
}

void Sphere::CreateSphere(GLfloat* vertices, GLfloat* normals, GLfloat* texcoords, GLuint* indices, float radius)
{
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);

	int index = 0;
	int uvIndex = 0;
	int indiceIndex = 0;
	for (int r = 0; r < rings; ++r) {
		for (int s = 0; s < sectors; ++s) {
			float const y = sin(-M_PI_2 + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			texcoords[uvIndex] = s * S;
			texcoords[uvIndex + 1] = r * R;

			vertices[index] = x * radius;
			vertices[index + 1] = y * radius;
			vertices[index + 2] = z * radius;

			normals[index] = x;
			normals[index + 1] = y;
			normals[index + 2] = z;

			if (r < rings - 1)
				push_indices(indices, indiceIndex, sectors, r, s);

			indiceIndex++;
			index += 3;
			uvIndex += 2;
		}
	}
}

inline void Sphere::push_indices(GLuint* indices, int index, int sectors, int r, int s) {
	index *= 6;
	int curRow = r * sectors;
	int nextRow = (r + 1) * sectors;
	int nextS = (s + 1) % sectors;

	indices[index] = curRow + s;
	indices[index + 1] = nextRow + s;
	indices[index + 2] = nextRow + nextS;

	indices[index + 3] = curRow + s;
	indices[index + 4] = nextRow + nextS;
	indices[index + 5] = curRow + nextS;
}

void Sphere::SetColor(glm::vec4 color)
{
    _color = color;
}

//Sphere::~Sphere(void) {
//    glDeleteBuffers(1, &vbo_sphere_vertices);
//    glDeleteBuffers(1, &ibo_sphere_elements);
//}

void Sphere::BindBuffers(GLuint *vbo,GLuint *ibo)
{
    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices_, GL_STATIC_DRAW);

    glGenBuffers(1, ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices_, GL_STATIC_DRAW);
}
