#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight() { lightType = LightType::DIRECTIONAL; }
	~DirectionalLight() {}

	vec3 direction = { 0, -1, 0 };
};
