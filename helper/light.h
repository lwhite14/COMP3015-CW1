#ifndef Light_H
#define Light_H

#include <glm/glm.hpp>
using glm::vec3;
using glm::vec4;

class Light 
{
public:
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	vec3 direction;
	float exponent;
	float cutoff;

	Light() {}

	Light(vec4 newPos, vec3 newAm, vec3 newDiff, vec3 newSpec) 
	{
		position = newPos;
		ambient = newAm;
		diffuse = newDiff;
		specular = newSpec;

		direction = vec3(0, 0, 0);
		exponent = 0.0f;
		cutoff = 0.0f;
	}

	Light(vec4 newPos, vec3 newAm, vec3 newDiff, vec3 newSpec, vec3 newDir, float newExp, float newCut)
	{
		position = newPos;
		ambient = newAm;
		diffuse = newDiff;
		specular = newSpec;
		direction = newDir;
		exponent = newExp;
		cutoff = newCut;
	}
};


#endif // Light_H
