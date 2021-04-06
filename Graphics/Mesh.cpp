#include "Mesh.h"
#include <gl_core_4_4.h>

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vertexArrayObjects);
	glDeleteBuffers(1, &vertexBufferObjects);
	glDeleteBuffers(1, &indexBufferObjects);
}

void Mesh::InitialiseQuad()
{
	// check that the mesh is not initialized already
	assert(vertexArrayObjects == 0);

	// generate buffers
	glGenBuffers(1, &vertexBufferObjects);
	glGenVertexArrays(1, &vertexArrayObjects);

	// bind vertex array aka a mesh wrapper
	glBindVertexArray(vertexArrayObjects);

	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects);

	// define 6 vertices for 2 triangles
	Vertex vertices[6];
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };

	vertices[3].position = { -0.5f, 0, -0.5f, 1 };
	vertices[4].position = { 0.5f, 0, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0, -0.5f, 1 };

	// add coordinates to the vertices
	vertices[0].texCoord = { 0, 1 }; // bottom left
	vertices[1].texCoord = { 1, 1 }; // bottom right
	vertices[2].texCoord = { 0, 0 }; // top left

	vertices[3].texCoord = { 0, 0 }; // top left
	vertices[4].texCoord = { 1, 1 }; // bottom right
	vertices[5].texCoord = { 1, 0 }; // top right

	// add normals
	vertices[0].normal = { 0, 1, 0, 0 };
	vertices[1].normal = { 0, 1, 0, 0 };
	vertices[2].normal = { 0, 1, 0, 0 };
	vertices[3].normal = { 0, 1, 0, 0 };
	vertices[4].normal = { 0, 1, 0, 0 };
	vertices[5].normal = { 0, 1, 0, 0 };

	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// enable second element as normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	// enable third element as texture
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// quad has 2 tris
	triCount = 2;
}

void Mesh::InitialiseFullScreenQuad()
{
	// check that the mesh is not initialized already
	assert(vertexArrayObjects == 0);

	// generate buffers
	glGenBuffers(1, &vertexBufferObjects);
	glGenVertexArrays(1, &vertexArrayObjects);

	// bind vertex array aka a mesh wrapper
	glBindVertexArray(vertexArrayObjects);

	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects);

	// define vertices
	float vertices[] =
	{
		-1, 1, // left top
		-1, -1, // left bottom
		1, 1, // right top
		-1, -1, // left bottom
		1, -1, // right bottom
		1, 1, // right top
	};

	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0);

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// quad has 2 tris
	triCount = 2;
}

void Mesh::Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount, unsigned int* indices)
{
	assert(vertexArrayObjects == 0);

	// generate buffers
	glGenBuffers(1, &vertexBufferObjects);
	glGenVertexArrays(1, &vertexArrayObjects);

	// bind vertex array aka a mesh wrapper
	glBindVertexArray(vertexArrayObjects);

	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects);

	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// bind indices if there are any
	if (indexCount != 0)
	{
		glGenBuffers(1, &indexBufferObjects);

		// bind vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjects);

		// fill vertex buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		triCount = indexCount / 3;
	}
	else
	{
		triCount = vertexCount / 3;
	}

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Draw()
{
	glBindVertexArray(vertexArrayObjects);

	if (indexBufferObjects != 0)
		glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
}
