#include "Camera.h"
#include <glm\ext.hpp>

void Camera::SetPerspective(float fieldOfView, float aspectRatio, float near, float far)
{
	projectionTransform = (fieldOfView) ?
		perspective(fieldOfView, aspectRatio, near, far) :
		ortho(16.0f, 16.0f, 9.0f, 9.0f, 0.1f, 50.0f);
}

void Camera::SetLookAt(vec3 from, vec3 to, vec3 up)
{
	position = from;
	forward = normalize(to - from);
	viewTransform = lookAt(from, to, up);
}

void Camera::SetPosition(vec3 a_position)
{
	SetLookAt(position, position + forward, up);
}

void Camera::GetFrustumPlanes(const mat4& transform, vec4* planes)
{
	// Right side
	planes[0] = vec4(transform[0][3] - transform[0][0],
		transform[1][3] - transform[1][0],
		transform[2][3] - transform[2][0],
		transform[3][3] - transform[3][0]
	);

	// Left side
	planes[1] = vec4(transform[0][3] + transform[0][0],
		transform[1][3] + transform[1][0],
		transform[2][3] + transform[2][0],
		transform[3][3] + transform[3][0]
	);

	// Top
	planes[2] = vec4(transform[0][3] - transform[0][1],
		transform[1][3] - transform[1][1],
		transform[2][3] - transform[2][1],
		transform[3][3] - transform[3][1]
	);

	// Bottom
	planes[3] = vec4(transform[0][3] + transform[0][1],
		transform[1][3] + transform[1][1],
		transform[2][3] + transform[2][1],
		transform[3][3] + transform[3][1]
	);

	// Far
	planes[4] = vec4(transform[0][3] - transform[0][2],
		transform[1][3] - transform[1][2],
		transform[2][3] - transform[2][2],
		transform[3][3] - transform[3][2]
	);

	// Near
	planes[4] = vec4(transform[0][3] + transform[0][2],
		transform[1][3] + transform[1][2],
		transform[2][3] + transform[2][2],
		transform[3][3] + transform[3][2]
	);

	// Normalise planes
	for (int i = 0; i < 6; ++i)
	{
		float d = length(vec3(planes[i]));
		planes[i] /= d;
	}
}

void Camera::UpdateWorldTransform()
{
	worldTransform = inverse(viewTransform);
}

void Camera::UpdateProjectionViewTransform()
{
	projectionViewTransform = projectionTransform * viewTransform;
}
