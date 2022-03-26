#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <glm/glm.hpp>
using glm::vec3;
using glm::vec4;

class PointLight 
{
public:
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	PointLight() {}

	PointLight(vec4 newPos, vec3 newAm, vec3 newDiff, vec3 newSpec) 
	{
		position = newPos;
		ambient = newAm;
		diffuse = newDiff;
		specular = newSpec;
	}
};


#endif // POINTLIGHT_H
