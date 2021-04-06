#pragma once
#include "Light.h"

class SpotLight : public Light
{
public:
	SpotLight() { lightType = LightType::SPOTLIGHT; }
	~SpotLight() {}

	float cutoff = 0;
};
