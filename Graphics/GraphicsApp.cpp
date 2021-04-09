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
	standardLight.diffuse = { 0.5f, 0.5f, 0.0f };
	directionalLight.diffuse = { 1.0f, 1.0f, 1.0f };
	pointLight1.diffuse = { 0.5f, 0.5f, 0.0f };
	pointLight1.diffuseStrength = 5;
	pointLight2.diffuse = { 0.0f, 0.0f, 1.0f };
	pointLight2.diffuseStrength = 5;
	pointLight3.diffuse = { 0.0f, 1.0f, 1.0f };
	pointLight3.diffuseStrength = 5;
	pointLight4.diffuse = { 1.0f, 0.0f, 0.0f };
	pointLight4.diffuseStrength = 5;

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
	dragon.LoadMesh("./stanford/dragon.obj");
	spear.LoadMesh("./soulspear/soulspear.obj", true, true);
	statuette.LoadMesh("./statuette/statuette.obj", true, true);

	// initialise object transforms
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

	// rotate the point lights
	pointLight1.position = vec3(cosf(time) * 2, 5, sinf(time) * 2);
	pointLight1.position += statuette.GetPosition();
	pointLight2.position = vec3(-cosf(time) * 5, 2, -sinf(time) * 5);
	pointLight2.position += statuette.GetPosition();
	pointLight3.position = vec3(cosf(time), 2, sinf(time));
	pointLight3.position += spear.GetPosition();
	pointLight4.position = vec3(-cosf(time), 2, -sinf(time));
	pointLight4.position += spear.GetPosition();

	// update the camera
	flyCam.Update(deltaTime);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->wasMouseButtonPressed(INPUT_MOUSE_BUTTON_LEFT))
	{
		postIndex = (postIndex < 3) ? postIndex + 1 : 0;
	}

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

	// send camera's position to shader as uniform
	normalShader.bindUniform("cameraPosition", flyCam.GetPosition());

	// bind directional light's variables
	normalShader.bindUniform("dirLights[0].ambient", directionalLight.ambient);
	normalShader.bindUniform("dirLights[0].diffuse", directionalLight.diffuse);
	normalShader.bindUniform("dirLights[0].specular", directionalLight.specular);
	normalShader.bindUniform("dirLights[0].ambientStrength", directionalLight.ambientStrength);
	normalShader.bindUniform("dirLights[0].diffuseStrength", directionalLight.diffuseStrength);
	normalShader.bindUniform("dirLights[0].specularStrength", directionalLight.specularStrength);
	normalShader.bindUniform("dirLights[0].specularPower", directionalLight.specularPower);
	normalShader.bindUniform("dirLights[0].direction", directionalLight.direction);

	// send point light's position to shader as uniform
	normalShader.bindUniform("pointLights[0].position", pointLight1.position);

	// bind point light's variables
	normalShader.bindUniform("pointLights[0].ambient", pointLight1.ambient);
	normalShader.bindUniform("pointLights[0].diffuse", pointLight1.diffuse);
	normalShader.bindUniform("pointLights[0].specular", pointLight1.specular);
	normalShader.bindUniform("pointLights[0].ambientStrength", pointLight1.ambientStrength);
	normalShader.bindUniform("pointLights[0].diffuseStrength", pointLight1.diffuseStrength);
	normalShader.bindUniform("pointLights[0].specularStrength", pointLight1.specularStrength);
	normalShader.bindUniform("pointLights[0].specularPower", pointLight1.specularPower);
	normalShader.bindUniform("pointLights[0].constant", pointLight1.constant);
	normalShader.bindUniform("pointLights[0].linear", pointLight1.linear);
	normalShader.bindUniform("pointLights[0].quadratic", pointLight1.quadratic);

	// send point light's position to shader as uniform
	normalShader.bindUniform("pointLights[1].position", pointLight2.position);

	// bind point light's variables
	normalShader.bindUniform("pointLights[1].ambient", pointLight2.ambient);
	normalShader.bindUniform("pointLights[1].diffuse", pointLight2.diffuse);
	normalShader.bindUniform("pointLights[1].specular", pointLight2.specular);
	normalShader.bindUniform("pointLights[1].ambientStrength", pointLight2.ambientStrength);
	normalShader.bindUniform("pointLights[1].diffuseStrength", pointLight2.diffuseStrength);
	normalShader.bindUniform("pointLights[1].specularStrength", pointLight2.specularStrength);
	normalShader.bindUniform("pointLights[1].specularPower", pointLight2.specularPower);
	normalShader.bindUniform("pointLights[1].constant", pointLight2.constant);
	normalShader.bindUniform("pointLights[1].linear", pointLight2.linear);
	normalShader.bindUniform("pointLights[1].quadratic", pointLight2.quadratic);

	// send point light's position to shader as uniform
	normalShader.bindUniform("pointLights[2].position", pointLight3.position);

	// bind point light's variables
	normalShader.bindUniform("pointLights[2].ambient", pointLight3.ambient);
	normalShader.bindUniform("pointLights[2].diffuse", pointLight3.diffuse);
	normalShader.bindUniform("pointLights[2].specular", pointLight3.specular);
	normalShader.bindUniform("pointLights[2].ambientStrength", pointLight3.ambientStrength);
	normalShader.bindUniform("pointLights[2].diffuseStrength", pointLight3.diffuseStrength);
	normalShader.bindUniform("pointLights[2].specularStrength", pointLight3.specularStrength);
	normalShader.bindUniform("pointLights[2].specularPower", pointLight3.specularPower);
	normalShader.bindUniform("pointLights[2].constant", pointLight3.constant);
	normalShader.bindUniform("pointLights[2].linear", pointLight3.linear);
	normalShader.bindUniform("pointLights[2].quadratic", pointLight3.quadratic);

	// send point light's position to shader as uniform
	normalShader.bindUniform("pointLights[3].position", pointLight4.position);

	// bind point light's variables
	normalShader.bindUniform("pointLights[3].ambient", pointLight4.ambient);
	normalShader.bindUniform("pointLights[3].diffuse", pointLight4.diffuse);
	normalShader.bindUniform("pointLights[3].specular", pointLight4.specular);
	normalShader.bindUniform("pointLights[3].ambientStrength", pointLight4.ambientStrength);
	normalShader.bindUniform("pointLights[3].diffuseStrength", pointLight4.diffuseStrength);
	normalShader.bindUniform("pointLights[3].specularStrength", pointLight4.specularStrength);
	normalShader.bindUniform("pointLights[3].specularPower", pointLight4.specularPower);
	normalShader.bindUniform("pointLights[3].constant", pointLight4.constant);
	normalShader.bindUniform("pointLights[3].linear", pointLight4.linear);
	normalShader.bindUniform("pointLights[3].quadratic", pointLight4.quadratic);

	// bind transform
	normalShader.bindUniform("ProjectionViewModel", spear.GetProjectionViewMatrix(&flyCam));

	// bind transforms for lighting
	normalShader.bindUniform("ModelMatrix", spear.transform);
	normalShader.bindUniform("NormalMatrix", inverseTranspose(mat3(spear.transform)));

	// draw the spear
	spear.Draw();

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
	postShader.bindUniform("postEffect", postIndex); // 0: default, 1: blur, 2: distort, 3: sobel
	postShader.bindUniform("colourTarget", 0);
	fullScreenRenderTarget.getTarget(0).bind(0);

	// draw the full screen quad
	fullScreenQuadMesh.Draw();

	// draw the lights
	standardLight.Draw();
	pointLight1.Draw();
	pointLight2.Draw();
	pointLight3.Draw();
	pointLight4.Draw();

	Gizmos::draw(flyCam.GetProjectionViewTransform());
}
