#include <stdlib.h>
#include <stdio.h>
#include "globals.h"

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