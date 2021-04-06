#include "Light.h"
#include <Gizmos.h>

using aie::Gizmos;

void Light::Draw()
{
	Gizmos::addSphere(position, 0.2f, 12, 12, vec4(diffuse, 1));
}
