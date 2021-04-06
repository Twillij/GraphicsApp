#pragma once
#include "FlyCamera.h"
#include <Application.h>
#include <glm/mat4x4.hpp>

class GraphicsApp : public aie::Application
{
public:

	GraphicsApp() {}
	virtual ~GraphicsApp() {}

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	// camera transforms
	mat4 viewMatrix = mat4(1);
	mat4 projectionMatrix = mat4(1);

	FlyCamera flyCam;
};
