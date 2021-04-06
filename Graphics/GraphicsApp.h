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

	FlyCamera flyCam;
};
