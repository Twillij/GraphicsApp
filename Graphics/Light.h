#pragma once
#include <glm/vec3.hpp>

using glm::vec3;

class Light
{
public:
	Light() {}
	~Light() {}

	vec3 direction = { 0, 0, 0 };
	vec3 diffuse = { 1, 1, 0 };
	vec3 specular = { 1, 1, 0 };
};
