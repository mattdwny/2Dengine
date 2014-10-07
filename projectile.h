#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "graphics.h"
#include "vector2.h"
#include "AABB.h"

typedef struct Projectile_S
{
	Sprite* sprite;
	int frame;
	vec2d pos;
	vec2d vel;
	AABB BBox;
	float radius;
} Projectile;

void* ProjectileInit(void* data);
void  ProjectileDestroy(void* data);
void  ProjectileDraw(void* data);
void  ProjectileThink(void* data);
void  ProjectileUpdate(void* data);

#endif