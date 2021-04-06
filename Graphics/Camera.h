#pragma once
#include <glm/matrix.hpp>

using namespace glm;

class Camera
{
public:
	virtual void Update(float deltaTime) = 0;

	void SetPerspective(float fieldOfView, float aspectRatio, float near, float far);
	void SetLookAt(vec3 from, vec3 to, vec3 up);

	vec3 GetPosition() { return position; }
	void SetPosition(vec3 a_position);

	mat4 GetWorldTransform() { return worldTransform; }
	mat4 GetViewTransform() { return viewTransform; }
	mat4 GetProjectionTransform() { return projectionTransform; }
	mat4 GetProjectionViewTransform() { return projectionViewTransform; }

	void GetFrustumPlanes(const mat4& transform, vec4* planes);

private:
	mat4 worldTransform;
	mat4 viewTransform;
	mat4 projectionTransform;
	mat4 projectionViewTransform;

	vec3 position;
	vec3 forward;
	vec3 right;
	vec3 up = vec3(0, 1, 0);

	void UpdateWorldTransform();
	void UpdateProjectionViewTransform();
};
