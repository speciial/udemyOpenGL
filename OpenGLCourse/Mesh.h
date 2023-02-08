#pragma once

#include <GL/glew.h>

class Mesh
{
public:
	Mesh();
	~Mesh();

	void Create(GLfloat *vertices, GLuint *indices, GLuint vertexCount, GLuint indexCount);
	void Render();
	void Clear();

private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLsizei indexCount;
};

