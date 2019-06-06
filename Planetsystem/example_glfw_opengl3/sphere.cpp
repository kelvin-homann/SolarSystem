#include "sphere.h"
#include "soil.h"

// Constructor
Sphere::Sphere(float radius) {
	float _radius = radius;
	CreateSphere(vertices_, normals_, texcoords_, indices_, _radius);
}

Sphere::~Sphere(void) {
    glDeleteBuffers(1, &_vbo_vertices);
    glDeleteBuffers(1, &_ibo);
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

void Sphere::BindShader()
{
    GetShader().Use(); // glUseProgram
}

void Sphere::Render()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_vertices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_normals);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_textures);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    int size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
}

void Sphere::BindBuffers()
{
    glGenBuffers(1, &_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices_, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenBuffers(1, &_vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices_, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &_vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, normals_size, normals_, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &_vbo_textures);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_textures);
    glBufferData(GL_ARRAY_BUFFER, texcoords_size, texcoords_, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
