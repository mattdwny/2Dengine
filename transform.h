#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vector2.h"
#include "AABB.h"

typedef struct Transform_S
{
	AABB rect;
	vec2d pos;
	vec2d vel;
	float radius;
} Transform;

#endif