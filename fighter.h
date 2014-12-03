#ifndef FIGHTER_H
#define FIGHTER_H

#include "graphics.h"
#include "vector2.h"
#include "AABB.h"
#include "controller.h"

typedef enum { STAND = 0, CROUCH, MOVE, FALL, FASTFALL, ROLL, SPOT_DODGE, AIR_DODGE, TUMBLE, GROUND_STUN, AIR_STUN, FLOOR_TECH, CEIL_TECH, LWALL_TECH, RWALL_TECH, BLOCK, GRAB, AIR_GRAB, GRABBED } FighterState;

typedef struct Fighter_S
{
	FighterState fightState;
	Controller* controller;
	Sprite* sprite;
	float frame;
	vec2d pos;
	vec2d vel;
	AABB BBox;
	float radius;
	int maxJumps;
	int curJumps;
	float jumpSpeed;
	float runSpeed;
	float stopSpeed;
	float runAccel;
	float friction;
	float gravity;
	float fallSpeed;
	float fastFallSpeed;
} Fighter;

void* FighterInit(void* data);
void  FighterDestroy(void* data);
void  FighterDraw(void* data);
void  FighterThink(void* data);
void  FighterUpdate(void* data);

#endif