#include "GraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using aie::Gizmos;
using glm::vec3;
using glm::vec4;
using glm::mat4;

bool GraphicsApp::startup()
{
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// initialise main camera
	flyCam.SetSpeed(5.0f);
	flyCam.SetPerspective(radians(45.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
	flyCam.SetLookAt(vec3(10, 10, 10), vec3(0, 0, 5), vec3(0, 1, 0));

	return true;
}

void GraphicsApp::shutdown()
{
	Gizmos::destroy();
}

void GraphicsApp::update(float deltaTime)
{
	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// update the camera
	flyCam.Update(deltaTime);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsApp::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	Gizmos::draw(flyCam.GetProjectionViewTransform());
}
