#include "fighter.h"

#define __maxFighters 2

Fighter* fighters[__maxFighters];

void InitFighter(void* voidptr)
{
	Fighter* fighter = (Fighter*) voidptr;

	fighter->fightState = STAND;

	//...
}


void DrawFighter()
{

}

void FreeFighter(Fighter* fighter)
{

}

void FighterThink(Fighter* fighterInfo)
{
	switch(fighterInfo->fightState)
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