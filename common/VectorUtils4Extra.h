#pragma once
#include "VectorUtils4.h"

// VectorUtils4 extensions by Terence.
// Written 25/3/2024
// Updated 25/4/2024
// You must use this in conjunction with Vector4Utils

vec3 ElementMult(vec3 a, vec3 b); // 25/3/2024, Terence, ADDED: element wise multiply operation
GLfloat NormSq(vec3 a);			  // 25/3/2024, Terence, ADDED: norm squared

// ********** implementation section ************

#ifdef MAIN
vec3 ElementMult(vec3 a, vec3 b)
{
	vec3 result;

	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;

	return result;
}

GLfloat NormSq(vec3 a)
{
	GLfloat result;

	result = (GLfloat)(a.x * a.x + a.y * a.y + a.z * a.z);
	return result;
}


#endif
