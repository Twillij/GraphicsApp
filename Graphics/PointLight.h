#pragma once
#include "Light.h"

/* attenuation table
Distance	Constant 	Linear 	Quadratic
7 			1.0 		0.7 	1.8
13 			1.0 		0.35 	0.44
20 			1.0 		0.22 	0.20
32 			1.0 		0.14 	0.07
50			1.0 		0.09 	0.032
65			1.0 		0.07 	0.017
100 		1.0 		0.045 	0.0075
160 		1.0 		0.027 	0.0028
200 		1.0 		0.022 	0.0019
325 		1.0 		0.014 	0.0007
600 		1.0 		0.007 	0.0002
3250 		1.0 		0.0014 	0.000007
*/

class PointLight : public Light
{
public:
	PointLight() { lightType = LightType::POINTLIGHT; }
	~PointLight() {}

	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};
