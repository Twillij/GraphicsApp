#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using namespace glm;

enum LightType
{
	STANDARD,
	DIRECTIONAL,
	POINTLIGHT,
	SPOTLIGHT
};

class Light
{
public:
	Light() {}
	~Light() {}

	vec3 position = vec3(0);

	vec3 ambient = { 0.25f, 0.25f, 0.25f };
	vec3 diffuse = { 1, 1, 1 };
	vec3 specular = { 1, 1, 0 };

	float ambientStrength = 0.1f;
	float specularStrength = 0.5f;
	int specularPower = 32;

	int GetLightType() { return lightType; }

	virtual void Draw();

protected:
	int lightType = LightType::STANDARD;
};
