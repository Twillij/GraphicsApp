#pragma once
#include "FlyCamera.h"
#include "RenderObject.h"
#include "Shader.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
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

	// camera
	FlyCamera flyCam;

	// shaders
	ShaderProgram phongShader;
	ShaderProgram normalShader;
	ShaderProgram postShader;

	// lights
	Light standardLight;
	DirectionalLight directionalLight;
	PointLight pointLight;
	SpotLight spotLight;

	// render objects
	RenderObject angel;
	RenderObject dragon;
	RenderObject spear;
	RenderObject statuette;
};
