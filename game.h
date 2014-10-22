#ifndef GAME_H
#define GAME_H

#include <stdio.h>

//definition in a header file! GASP
void CRASH(char* msg) //a simple implementation to make the code more concise. Turns four lines into one.
{
	fprintf(stderr, msg);
	exit(-1);
}

#endif