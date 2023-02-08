#include "Mesh.h"

Mesh::Mesh() 
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
	indexCount = 0;
}

Mesh::~Mesh()
{
	Clear();
}

void Mesh::Create(GLfloat* vertices, GLuint* indices, GLuint vertexCount, GLuint indexCount)
{
	this->indexCount = indexCount;

	// create and bind the vao
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// create and bind the ebo
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// upload the indecies to the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indexCount, indices, GL_STATIC_DRAW);

	// create and bind the vbo 
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// upload the data to the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertexCount, vertices, GL_STATIC_DRAW);

	// specify location and format of the uploaded data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// unbinding the bound buffers
	// NOTE(christian): according to the tutorial, you should unbind the vao before the ebo and vbo
	//					which would make sense. otherwise the ebo and vbo would be unbound in the 
	//					vao as well. check this!
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Render()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::Clear()
{
	// NOTE(christian): how does memory management work in opengl? 
	if (EBO != 0)
	{
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}