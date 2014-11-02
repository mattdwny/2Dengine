#include "fighter.h"
#include "globals.h"

#define __maxFighters 2

Fighter* fighters[__maxFighters];



void* FighterInit(void* data)
{
	if(!data) return NULL;

	Fighter* fighter = (Fighter*) data;

	fighter->fightState = STAND;

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

	fighter->frame = (fighter->frame + 1) % fighter->sprite->frames;
}

void FighterThink(void* data)
{
	if(!data) return;

	Fighter* fighter = (Fighter*) data;

	//...

	switch(fighter->fightState)
	{
		case STAND:
			//to crouch

			//to block

			//to move

			//to jump

			break;
		case BLOCK:
			//to grab
			//to stand
			//to roll
			break;
		case ROLL:
			//to block
			//to stand
			//to move
			break;
		case SPOT_DODGE:
			//to stand
			break;
		case GRAB:
			//to stand
			break;
		case CROUCH:
			//to stand
			//to jump
			//to grab
			break;
		case MOVE:
			//to jump
			//to stand
			//to grab
			break;
		case FALL:
			//to fastfall
			//to move
			//to stand
			//to air dodge
			//to air grab
			break;
		case FASTFALL:
			break;
		case AIR_DODGE:
			break;
		case TUMBLE:
			break;
		case GROUND_STUN:
			break;
		case AIR_STUN:
			break;
		case TECH:
			break;
		case AIR_GRAB:
			break;
		case GRABBED:
			break;
	}
}

void FighterUpdate(void* data)
{
	if(!data) return;

	Fighter* fighter = (Fighter*) data;

	//...
}