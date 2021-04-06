#pragma once
#include "Camera.h"

class FlyCamera : public Camera
{
public:
	void Update(float deltaTime);
	void SetSpeed(float speed) { this->speed = speed; }

private:
	float speed;
};
