#include "GraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace std;
using aie::Gizmos;

bool GraphicsApp::startup()
{
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// initialise main camera
	flyCam.SetSpeed(5.0f);
	flyCam.SetPerspective(radians(45.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
	flyCam.SetLookAt(vec3(10, 10, 10), vec3(0, 0, 5), vec3(0, 1, 0));

	// initialise lights
	standardLight.position = { 0, 5, 0 };
	standardLight.diffuse = { 1, 0, 1 };
	directionalLight.ambient = { 0.5f, 0.5f, 0.5f };
	directionalLight.ambientStrength = 0.3f;
	directionalLight.diffuse = { 1, 1, 0 };
	pointLight.position = { -5, 10, 0 };
	pointLight.diffuse = { 1, 1, 1 };

	// load shaders
	phongShader.loadShader(eShaderStage::VERTEX, "./shaders/phong.vert");
	phongShader.loadShader(eShaderStage::FRAGMENT, "./shaders/phong.frag");
	normalShader.loadShader(eShaderStage::VERTEX, "./shaders/normalmap.vert");
	normalShader.loadShader(eShaderStage::FRAGMENT, "./shaders/normalmap.frag");
	postShader.loadShader(eShaderStage::VERTEX, "./shaders/post.vert");
	postShader.loadShader(eShaderStage::FRAGMENT, "./shaders/post.frag");

	// link shaders
	if (phongShader.link() == false)
	{
		cout << "Phong shader link error: " << phongShader.getLastError() << endl;
		return false;
	}
	if (normalShader.link() == false)
	{
		cout << "Normal map shader link error: " << normalShader.getLastError() << endl;
		return false;
	}
	if (postShader.link() == false)
	{
		cout << "Post shader link error: " << postShader.getLastError() << endl;
		return false;
	}

	// initialise render targets
	if (fullScreenRenderTarget.initialise(1, getWindowWidth(), getWindowHeight()) == false)
	{
		cout << "Full screen target error!" << endl;
		return false;
	}

	// initialise meshes for target rendering
	fullScreenQuadMesh.InitialiseFullScreenQuad();

	// initiliase object meshes
	angel.LoadMesh("./stanford/lucy.obj");
	dragon.LoadMesh("./stanford/dragon.obj");
	spear.LoadMesh("./soulspear/soulspear.obj", true, true);
	statuette.LoadMesh("./statuette/statuette.obj", true, true);

	// initialise object transforms
	angel.transform =
	{
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		-5, 0, 5, 1
	};
	dragon.transform =
	{
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, -5, 1
	};
	spear.transform =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		5, 0, 5, 1
	};
	statuette.transform =
	{
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		-5, 0, 5, 1
	};

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

	// query time since app started
	float time = getTime();

	// rotate the directional light's direction
	directionalLight.direction = normalize(vec3(cosf(time * 2), sinf(time * 2), 0));

	// rotate the point light
	pointLight.position = { -5, 5, cosf(time * 2) * -3 + 10 };

	// update the camera
	flyCam.Update(deltaTime);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsApp::draw()
{
	// bind the render target
	fullScreenRenderTarget.bind();

	// wipe the screen to the background colour
	clearScreen();

	// bind phong shader program
	phongShader.bind();

	// indicate the light type to the shader
	phongShader.bindUniform("lightType", standardLight.GetLightType());

	// send light's position to shader as uniform
	phongShader.bindUniform("lightPosition", standardLight.position);

	// send camera's position to shader as uniform
	phongShader.bindUniform("cameraPosition", flyCam.GetPosition());

	// bind light
	phongShader.bindUniform("Ia", standardLight.ambient);
	phongShader.bindUniform("Id", standardLight.diffuse);
	phongShader.bindUniform("Is", standardLight.specular);
	phongShader.bindUniform("ambientStrength", standardLight.ambientStrength);
	phongShader.bindUniform("specularStrength", standardLight.specularStrength);
	phongShader.bindUniform("specularPower", standardLight.specularPower);

	// bind transform
	phongShader.bindUniform("ProjectionViewModel", dragon.GetProjectionViewMatrix(&flyCam));

	// bind transforms for lighting
	phongShader.bindUniform("ModelMatrix", dragon.transform);
	phongShader.bindUniform("NormalMatrix", inverseTranspose(mat3(dragon.transform)));

	// draw the dragon
	dragon.Draw();

	// bind normal map shader program
	normalShader.bind();

	// indicate the light type to the shader
	normalShader.bindUniform("lightType", directionalLight.GetLightType());

	// send camera's position to shader as uniform
	normalShader.bindUniform("cameraPosition", flyCam.GetPosition());

	// bind light
	normalShader.bindUniform("Ia", directionalLight.ambient);
	normalShader.bindUniform("Id", directionalLight.diffuse);
	normalShader.bindUniform("Is", directionalLight.specular);
	normalShader.bindUniform("ambientStrength", directionalLight.ambientStrength);
	normalShader.bindUniform("specularStrength", directionalLight.specularStrength);
	normalShader.bindUniform("specularPower", directionalLight.specularPower);
	normalShader.bindUniform("lightDirection", directionalLight.direction);

	// bind transform
	normalShader.bindUniform("ProjectionViewModel", spear.GetProjectionViewMatrix(&flyCam));

	// bind transforms for lighting
	normalShader.bindUniform("ModelMatrix", spear.transform);
	normalShader.bindUniform("NormalMatrix", inverseTranspose(mat3(spear.transform)));

	// draw the spear
	spear.Draw();

	// bind normal map shader program
	normalShader.bind();

	// indicate the light type to the shader
	normalShader.bindUniform("lightType", pointLight.GetLightType());

	// send light's position to shader as uniform
	normalShader.bindUniform("lightPosition", pointLight.position);

	// send camera's position to shader as uniform
	normalShader.bindUniform("cameraPosition", flyCam.GetPosition());
	
	// bind light
	normalShader.bindUniform("Ia", pointLight.ambient);
	normalShader.bindUniform("Id", pointLight.diffuse);
	normalShader.bindUniform("Is", pointLight.specular);
	normalShader.bindUniform("ambientStrength", pointLight.ambientStrength);
	normalShader.bindUniform("specularStrength", pointLight.specularStrength);
	normalShader.bindUniform("specularPower", pointLight.specularPower);
	normalShader.bindUniform("constant", pointLight.constant);
	normalShader.bindUniform("linear", pointLight.linear);
	normalShader.bindUniform("quadratic", pointLight.quadratic);

	// bind transform
	normalShader.bindUniform("ProjectionViewModel", statuette.GetProjectionViewMatrix(&flyCam));

	// bind transforms for lighting
	normalShader.bindUniform("ModelMatrix", statuette.transform);
	normalShader.bindUniform("NormalMatrix", inverseTranspose(mat3(statuette.transform)));

	// draw the spear
	statuette.Draw();

	// unbind target to return to backbuffer
	fullScreenRenderTarget.unbind();

	// clear the backbuffer
	clearScreen();

	// bind post shader and textures
	postShader.bind();
	postShader.bindUniform("postEffect", 3); // 0: default, 1: blur, 2: distort, 3: thick outline
	postShader.bindUniform("colourTarget", 0);
	fullScreenRenderTarget.getTarget(0).bind(0);

	// draw the full screen quad
	fullScreenQuadMesh.Draw();

	// draw the lights
	standardLight.Draw();
	pointLight.Draw();

	Gizmos::draw(flyCam.GetProjectionViewTransform());
}
