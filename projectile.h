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
	SDL_Rect BBox;
	float radius;
} Projectile;

void FreeProjectile(Projectile* proj);

#endif