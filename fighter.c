#include "fighter.h"
#include "globals.h"
#include "controller.h"

#define __maxFighters 2

//Hyper Sphere Fighter 4

#define sign(x) ((x > 0) - (x < 0)) //I can code
#define sqr(x) ((x)*(x))

extern SDL_Surface* screen;
extern Controller controllers[4];

Fighter* fighters[__maxFighters];
int controllerFree = 0;
int frameslow = 0;


void ApplyHalfGravity(Fighter* fighter);
void MovePlayer(Fighter* fighter);
void Clamp(Fighter* fighter);
void SpriteFrameHandler(Fighter* fighter);

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
	fighter->stopSpeed = 10;
	fighter->friction = 250;
	fighter->runAccel = 900;

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

	if(fighter == NULL) CRASH("Critical Error, nonexistant fighter");

	if(fighter->sprite == NULL) return; //nothing to draw

	Transform* trans = &fighter->trans;

	DrawSprite(fighter->sprite, (int) trans->pos[0] - 32, (int) trans->pos[1] - 32, (int) floor(fighter->frame)); //draw the player so he is centered, hence -32

	if(fighter->fightState == BLOCK) mult_circle(screen, (int) trans->pos[0], (int) trans->pos[1], 32, Black_);

	//fighter->frame += CROSS(

	SpriteFrameHandler(fighter);

	//printf("The frame number is %f\n", fighter->frame);

	switch(fighter->fightState)
	{
		case ROLL: case SPOT_DODGE: case AIR_DODGE: case FLOOR_TECH: case CEIL_TECH: case LWALL_TECH: case RWALL_TECH:
			//fighter->frame = (fighter->frame + 1) % fighter->sprite->frames;
			break;

		default:
			//if(frameslow % 4 == 0) fighter->frame = (fighter->frame + 1) % fighter->sprite->frames;
			break;
	}
}

void FighterThink(void* data)
{
	if(!data) return;

	Fighter* fighter = (Fighter*) data;

	//...

	Controller* controls = fighter->controller;
	Transform* trans = &fighter->trans;

	switch(fighter->fightState)
	{
		case STAND:
			//printf("standing\n");
			
			if(controls->axes[AXIS_MOVE_V] == -1)//to jump
			{
				fighter->fightState = FALL;
				trans->vel[AXIS_MOVE_V] = fighter->jumpSpeed;
			}
			else if(controls->axes[AXIS_MOVE_V] == 1) fighter->fightState = CROUCH; //to crouch
			else if(controls->buttons[BUTTON_GUARD]) fighter->fightState = BLOCK; //to block
			else if(controls->axes[AXIS_MOVE_H] != 0) fighter->fightState = MOVE; //to move

			break;
		case BLOCK:
			printf("blocking\n");
			//to grab
			if(!controls->buttons[BUTTON_GUARD]) fighter->fightState = STAND;
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
				trans->vel[AXIS_MOVE_V] = fighter->jumpSpeed;
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

	ApplyHalfGravity(fighter); //check it: http://www.niksula.hut.fi/~hkankaan/Homepages/gravity.html
	trans->pos[AXIS_MOVE_H] += trans->vel[AXIS_MOVE_H] * deltaTime;
	trans->pos[AXIS_MOVE_V] += trans->vel[AXIS_MOVE_V] * deltaTime;
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
	Transform* trans = &fighter->trans;
	switch(fighter->fightState)
	{
		case FALL: case AIR_DODGE: case TUMBLE: case AIR_STUN: case CEIL_TECH: case LWALL_TECH: case RWALL_TECH: case AIR_GRAB: //fast fall doesn't need gravity because it is already maxed out/terminal vel
			float factor;
			factor = fighter->gravity/2 * deltaTime;
			
			if(trans->vel[AXIS_MOVE_V] > fighter->fallSpeed*0.8f && trans->vel[AXIS_MOVE_V] < fighter->fallSpeed)
			{
				factor *= (fighter->fallSpeed - trans->vel[AXIS_MOVE_V])/fighter->fallSpeed * 2; 
			}
			trans->vel[AXIS_MOVE_V] += factor;

			if(trans->vel[AXIS_MOVE_V] > fighter->fallSpeed) trans->vel[AXIS_MOVE_V] = fighter->fallSpeed;
			break;
	}
}

void MovePlayer(Fighter* fighter)
{
	Controller* controls = fighter->controller;
	Transform* trans = &fighter->trans;

	switch(fighter->fightState)
	{
		case MOVE: 
			trans->vel[0] -= sign(trans->vel[0]) * fighter->friction * deltaTime; //apply friction when grounded
			if(controls->axes[0] == 0) trans->vel[0] -= sign(trans->vel[0]) * fighter->runAccel * deltaTime; //apply runAccel friction with no input
			//break intentionally omitted

		case FALL: case FASTFALL:
			if(controls->axes[0] != 0) trans->vel[0] += controls->axes[0] * fighter->runAccel * deltaTime; //apply player movement on MOVE, FALL, FASTFALL

			if     (trans->vel[0] >  fighter->runSpeed) trans->vel[0] =  fighter->runSpeed;
			else if(trans->vel[0] < -fighter->runSpeed) trans->vel[0] = -fighter->runSpeed;
			else if(abs(trans->vel[0]) < fighter->stopSpeed && controls->axes[0] == 0 && fighter->fightState == MOVE)
			{
				trans->vel[0] = 0;
				fighter->fightState = STAND;
			}
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

void SpriteFrameHandler(Fighter* fighter)
{
	Transform* trans = &fighter->trans;

	static vec2d center = { ResolutionX_*0.5, ResolutionY_*0.5 };
	vec2d toCenter;
	float cross;

	toCenter[0] = center[0] - trans->pos[0]; //find the direction of the player relative to the center
	toCenter[1] = center[1] - trans->pos[1];

	cross = toCenter[1]*trans->vel[0] - toCenter[0]*trans->vel[1];
	cross /= 100000;

	fighter->frame += cross;
	while(fighter->frame >= fighter->sprite->frames) fighter->frame -= fighter->sprite->frames; //clamp and float modulo frames into a valid range. If statements might be sufficient in the future.
	while(fighter->frame < 0)						 fighter->frame += fighter->sprite->frames;
}

void Clamp(Fighter* fighter)
{
	Transform* trans = &fighter->trans;

	if(trans->pos[1] > ResolutionY_ - 32)
	{
		trans->pos[1] = ResolutionY_ - 32;
		trans->vel[1] = 0;
		if(trans->vel[0] != 0) fighter->fightState = MOVE;
		else				   fighter->fightState = STAND;
	}
	if(trans->pos[0] < 32)
	{
		trans->pos[0] = 32;
		trans->vel[0] = 0;
		if(trans->pos[1] < ResolutionY_ - 32) fighter->fightState = FALL;
		else								  fighter->fightState = STAND;
	}
	if(trans->pos[0] > ResolutionX_ - 32)
	{
		trans->pos[0] = ResolutionX_ - 32;
		trans->vel[0] = 0;
		fighter->fightState = STAND;
		if(trans->pos[1] < ResolutionY_ - 32) fighter->fightState = FALL;
		else								  fighter->fightState = STAND;
	}
}