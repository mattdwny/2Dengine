#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "graphics.h"
#include "transform.h"
#include "vector2.h"
#include "AABB.h"

typedef struct Projectile_S
{
	Transform trans; //MUST be first
	int owner;
	Sprite* sprite;
	int frame;
	int frames;
	int color;
} Projectile;

void* ProjectileInit(void* data);
void  ProjectileDestroy(void* data);
void  ProjectileDraw(void* data);
void  ProjectileThink(void* data);
void  ProjectileUpdate(void* data);

#endif