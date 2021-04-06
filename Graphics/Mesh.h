#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

using namespace glm;

class Mesh
{
public:
	Mesh() : triCount(0), vertexArrayObjects(0), vertexBufferObjects(0), indexBufferObjects(0) {}
	virtual ~Mesh();

	struct Vertex
	{
		vec4 position;
		vec4 normal;
		vec2 texCoord;
	};

	void InitialiseQuad();
	void InitialiseFullScreenQuad();
	void Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);

	virtual void Draw();

protected:
	unsigned int triCount;
	unsigned int vertexArrayObjects;
	unsigned int vertexBufferObjects;
	unsigned int indexBufferObjects;
};
