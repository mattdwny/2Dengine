#ifndef GAME_H
#define GAME_H

#include <stdio.h>

#define ResolutionX_ 1024
#define ResolutionY_ 768

//I learned my lesson!
void CRASH(char* msg) //a simple implementation to make the code more concise. Turns four lines into one.
{
	fprintf(stderr, msg);
	exit(-1);
}

void CRASH(char* msg, char* err) //an overloaded method for SDL error printing
{
	fprintf(stderr, msg);
	fprintf(stderr, err);
	exit(-1);
}

#endif