#pragma once
#include "FlyCamera.h"
#include "Mesh.h"
#include "RenderObject.h"
#include "RenderTarget.h"
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
	PointLight pointLight1;
	PointLight pointLight2;
	PointLight pointLight3;
	PointLight pointLight4;
	SpotLight spotLight;

	// render objects
	RenderObject angel;
	RenderObject dragon;
	RenderObject spear;
	RenderObject statuette;

	// render targets
	RenderTarget fullScreenRenderTarget;

	// meshes for target rendering
	Mesh fullScreenQuadMesh;

	// post processing effect index
	int postIndex = 3;
};
