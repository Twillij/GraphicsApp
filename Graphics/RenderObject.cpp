#include "RenderObject.h"
#include <iostream>

using namespace std;

RenderObject::RenderObject()
{
	transform =
	{
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1
	};
}

vec3 RenderObject::GetPosition()
{
	return transform[3];
}

mat4 RenderObject::GetProjectionViewMatrix(Camera* camera)
{
    return camera->GetProjectionViewTransform() * transform;
}

bool RenderObject::LoadMesh(const char* filename, bool loadTextures, bool flipTextureV)
{
	if (mesh.load(filename, loadTextures, flipTextureV) == false)
	{
		cout << "Mesh load error" << endl;
		return false;
	}

	return true;
}

void RenderObject::Draw()
{
	mesh.draw();
}
