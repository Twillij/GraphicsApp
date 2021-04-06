#include "FlyCamera.h"
#include <Input.h>
#include <glm\gtx\rotate_vector.hpp>

using namespace aie;

void FlyCamera::Update(float deltaTime)
{
	Input* input = Input::getInstance();

	vec3 right = normalize(cross(up, -forward));
	vec3 localUp = normalize(cross(right, -forward));
	
	if (input->isKeyDown(INPUT_KEY_W))
		position += forward * speed * deltaTime;

	if (input->isKeyDown(INPUT_KEY_S))
		position -= forward * speed * deltaTime;

	if (input->isKeyDown(INPUT_KEY_A))
		position -= right * speed * deltaTime;

	if (input->isKeyDown(INPUT_KEY_D))
		position += right * speed * deltaTime;

	if (input->isKeyDown(INPUT_KEY_Z))
		position -= localUp * speed * deltaTime;

	if (input->isKeyDown(INPUT_KEY_X))
		position += localUp * speed * deltaTime;

	if (input->isMouseButtonDown(INPUT_MOUSE_BUTTON_RIGHT))
	{
		// get the current mouse coordinates
		float mouseX = (float)input->getMouseDeltaX();
		float mouseY = (float)input->getMouseDeltaY();

		forward = rotate(forward, radians(mouseX) * deltaTime * 5.0f, localUp);
		forward = rotate(forward, radians(mouseY) * deltaTime * 5.0f, right);
	}

	SetLookAt(position, position + forward, up);
	UpdateProjectionViewTransform();
	UpdateWorldTransform();
}
