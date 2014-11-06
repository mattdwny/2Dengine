#include "fighter.h"
#include "globals.h"
#include "controller.h"

#define __maxFighters 2

extern SDL_Surface* screen;
extern Controller controllers[4];

Fighter* fighters[__maxFighters];
int controllerFree = 0;
int frameslow = 0;


void ApplyHalfGravity(Fighter* fighter);
void MovePlayer(Fighter* fighter);
void Clamp(Fighter* fighter);

void* FighterInit(void* data)
{
	if(!data) return NULL;

	Fighter* fighter = (Fighter*) data;

	fighter->fightState = STAND;

	fighter->controller = &controllers[controllerFree++];

	fighter->maxJumps = 2;
	fighter->curJumps = 2;
	fighter->gravity = 128*9.8f/1.8f;
	fighter->fallSpeed = 700;
	fighter->fastFallSpeed = 1000;
	fighter->jumpSpeed = -550;
	fighter->runSpeed = 512;

	//...

	return data;
}

void FighterDestroy(void* data)
{
	if(!data) return;

	Fighter* fighter = (Fighter*) data;

	//...
}

void FighterDraw(void* data)
{
	Fighter* fighter;

	if(!data) return;

	fighter = (Fighter*) data;

	//fprintf(stderr, "Check: /%x/", fighter->sprite);

	if(fighter == NULL) CRASH("Critical Error, nonexistant fighter");

	if(fighter->sprite == NULL) return; //nothing to draw

	DrawSprite(fighter->sprite, (int) fighter->pos[0] - 32, (int) fighter->pos[1] - 32, fighter->frame); //draw the player so he is centered, hence -32

	if(fighter->fightState == FALL) mult_circle(screen, (int) fighter->pos[0], (int) fighter->pos[1], 32, Black_);

	switch(fighter->fightState)
	{
		case ROLL: case SPOT_DODGE: case AIR_DODGE: case FLOOR_TECH: case CEIL_TECH: case LWALL_TECH: case RWALL_TECH:
			fighter->frame = (fighter->frame + 1) % fighter->sprite->frames;
			break;

		default:
			if(frameslow % 4 == 0) fighter->frame = (fighter->frame + 1) % fighter->sprite->frames;
			break;
	}
}

void FighterThink(void* data)
{
	if(!data) return;

	Fighter* fighter = (Fighter*) data;

	//...

	Controller* controls = fighter->controller;

	switch(fighter->fightState)
	{
		case STAND:
			printf("standing\n");
			
			if(controls->axes[AXIS_MOVE_V] == -1)//to jump
			{
				fighter->fightState = FALL;
				fighter->vel[AXIS_MOVE_V] = fighter->jumpSpeed;
			}
			else if(controls->axes[AXIS_MOVE_V] == 1) fighter->fightState = CROUCH; //to crouch
			else if(controls->buttons[BUTTON_GUARD]) fighter->fightState = BLOCK; //to block
			else if(controls->axes[AXIS_MOVE_H] != 0) fighter->fightState = MOVE; //to move

			break;
		case BLOCK:
			printf("blocking\n");
			//to grab
			//to stand
			//to roll
			break;
		case ROLL:
			printf("rolling\n");
			//to block
			//to stand
			//to move
			break;
		case SPOT_DODGE:
			printf("spot dodging\n");
			//to stand
			break;
		case GRAB:
			printf("grabbing\n");
			//to stand
			break;
		case CROUCH:
			printf("crouching\n");
			//to stand
			//to jump
			//to grab
			break;
		case MOVE:
			printf("moving\n");
			if(controls->axes[AXIS_MOVE_V] == -1)//to jump
			{
				fighter->fightState = FALL;
				fighter->vel[AXIS_MOVE_V] = fighter->jumpSpeed;
			}
			//to jump
			//to stand
			//to grab
			break;
		case FALL:
			printf("falling\n");
			//to fastfall
			//to move
			//to stand
			//to air dodge
			//to air grab
			break;
		case FASTFALL:
			printf("fast falling\n");
			break;
		case AIR_DODGE:
			printf("air dodging\n");
			break;
		case TUMBLE:
			printf("tumblinging\n");
			break;
		case GROUND_STUN:
			printf("ground stunned\n");
			break;
		case AIR_STUN:
			printf("air stunned\n");
			break;
		case FLOOR_TECH:
			printf("ground teching\n");
			break;
		case CEIL_TECH:
			printf("ceil teching\n");
			break;
		case LWALL_TECH:
			printf("left teching\n");
			break;
		case RWALL_TECH:
			printf("right teching\n");
			break;
		case AIR_GRAB:
			printf("air grabbing\n");
			break;
		case GRABBED:
			printf("grabbed\n");
			break;
	}

	MovePlayer(fighter);

	//printf("%f\n",fighter->vel[1]);
	ApplyHalfGravity(fighter); //check it: http://www.niksula.hut.fi/~hkankaan/Homepages/gravity.html
	fighter->pos[AXIS_MOVE_H] += fighter->vel[AXIS_MOVE_H] * deltaTime;
	fighter->pos[AXIS_MOVE_V] += fighter->vel[AXIS_MOVE_V] * deltaTime;
	ApplyHalfGravity(fighter);

	Clamp(fighter);
}

void FighterUpdate(void* data)
{
	if(!data) return;

	Fighter* fighter = (Fighter*) data;

	//...
}

void ApplyHalfGravity(Fighter* fighter)
{
	switch(fighter->fightState)
	{
		case FALL: case AIR_DODGE: case TUMBLE: case AIR_STUN: case CEIL_TECH: case LWALL_TECH: case RWALL_TECH: case AIR_GRAB: //fast fall doesn't need gravity because it is already maxed out/terminal vel
			float factor;
			factor = fighter->gravity/2 * deltaTime;
			
			if(fighter->vel[AXIS_MOVE_V] > fighter->fallSpeed*0.8f && fighter->vel[AXIS_MOVE_V] < fighter->fallSpeed)
			{
				factor *= (fighter->fallSpeed - fighter->vel[AXIS_MOVE_V])/fighter->fallSpeed * 2; 
			}
			fighter->vel[AXIS_MOVE_V] += factor;

			if(fighter->vel[AXIS_MOVE_V] > fighter->fallSpeed) fighter->vel[AXIS_MOVE_V] = fighter->fallSpeed;
			break;
	}
}

void MovePlayer(Fighter* fighter)
{
	printf("happening");
	Controller* controls = fighter->controller;

	switch(fighter->fightState)
	{
		case MOVE: case FALL: case FASTFALL:
			if(controls->axes[AXIS_MOVE_H] != 0) fighter->vel[AXIS_MOVE_H] += controls->axes[AXIS_MOVE_H] * deltaTime * 700;
			//else								 fighter->vel[AXIS_MOVE_H] -= sign(
			printf("vel[AXIS_MOVE_H]: %f\n", fighter->vel[AXIS_MOVE_H]);
			printf("controls->axes[AXIS_MOVE_H]: %f\n", controls->axes[AXIS_MOVE_H]);
			
			if     (fighter->vel[AXIS_MOVE_H] >  fighter->runSpeed) fighter->vel[AXIS_MOVE_H] =  fighter->runSpeed;
			else if(fighter->vel[AXIS_MOVE_H] < -fighter->runSpeed) fighter->vel[AXIS_MOVE_H] = -fighter->runSpeed;
			break;
		case TUMBLE:
			break;
		case FLOOR_TECH:
			break;
		case CEIL_TECH:
			break;
		case LWALL_TECH:
			break;
		case RWALL_TECH:
			break;
	}
}

void Clamp(Fighter* fighter)
{
	if(fighter->pos[1] > ResolutionY_ - 32)
	{
		fighter->pos[1] = ResolutionY_ - 32;
		fighter->vel[1] = 0;
		if(fighter->vel[0] != 0) fighter->fightState = MOVE;
		else					 fighter->fightState = STAND;
	}
	if(fighter->pos[0] < 32)
	{
		fighter->pos[0] = 32;
		fighter->vel[0] = 0;
		if(fighter->pos[0] != ResolutionY_ - 32) fighter->fightState = FALL;
		else									 fighter->fightState = STAND;
	}
	if(fighter->pos[0] > ResolutionX_ - 32)
	{
		fighter->pos[0] = ResolutionX_ - 32;
		fighter->vel[0] = 0;
		fighter->fightState = STAND;
		if(fighter->pos[0] != ResolutionY_ - 32) fighter->fightState = FALL;
		else									 fighter->fightState = STAND;
	}
}