#pragma once
#include "Camera.h"
#include "OBJMesh.h"
#include "Shader.h"
#include <glm/matrix.hpp>

using namespace aie;

class RenderObject
{
public:
	RenderObject();
	~RenderObject() {}

	mat4 transform = mat4(1);
	OBJMesh mesh;

	mat4 GetProjectionViewMatrix(Camera* camera);

	bool LoadMesh(const char* filename, bool loadTextures = true, bool flipTextureV = false);

	virtual void Draw();
};
