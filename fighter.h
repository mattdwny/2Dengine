#ifndef FIGHTER_H
#define FIGHTER_H

#include "graphics.h"
#include "vector2.h"
#include "AABB.h"

typedef enum { STAND = 0, CROUCH, MOVE, FALL, FASTFALL, ROLL, SPOT_DODGE, AIR_DODGE, TUMBLE, GROUND_STUN, AIR_STUN, TECH, BLOCK, GRAB, AIR_GRAB, GRABBED } FighterState;

typedef struct Fighter_S
{
	FighterState fightState;

	Sprite* sprite;
	int frame;
	vec2d pos;
	vec2d vel;
	AABB BBox;
	float radius;
	int maxJumps;
} Fighter;

void* FighterInit(void* data);
void  FighterDestroy(void* data);
void  FighterDraw(void* data);
void  FighterThink(void* data);
void  FighterUpdate(void* data);

#endif