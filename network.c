#include "SDL.h"
#include "sdl_net.h"
#include "globals.h"
#include "entity.h"

extern Entity* __entityList; //volatile data due to multithreading

IPaddress addresses[2];
TCPsocket sockets[2];
char buffer[2][256]; //used for SENDER and RECEIVER
int player = 0;
int port = 7513;

//NOTE: I may have to recreate the code so that client and server are not mixed into the same code "network.c" but hopefully I don't to make using the network more abstract.
//Supplemental resources:
//
//Primary Reference: http://content.gpwiki.org/index.php/SDL:Tutorial:Using_SDL_net
//Best Network Function Documentation: http://www.libsdl.org/projects/SDL_net/docs/SDL_net.pdf
//Multithreading 1: http://lazyfoo.net/tutorials/SDL/46_multithreading/index.php
//Multithreading 2: http://lazyfoo.net/SDL_tutorials/lesson33/
//Multithreading Documentation: http://www.libsdl.org/release/SDL-1.2.15/docs/html/thread.html

void WaitNetwork();

/**
 *  @param host: if NULL the function will create a server, otherwise attempt to connect to "host" as a client
 */
void OpenNetwork(const char* host)
{
	int i;

	if(SDLNet_Init() == -1) CRASH("SDLNet_Init: ", SDLNet_GetError());

	if(host == NULL) player = 0;
	else			 player = 1;

	for(i = 0; i < 2; i++)
	{
		if(SDLNet_ResolveHost(&addresses[i], host, port+i) == -1) CRASH("SDLNet_ResolveHost: ", SDLNet_GetError()); 
		sockets[i] = SDLNet_TCP_Open(&addresses[i]);
		if(!sockets[i]) CRASH("SDLNet_TCP_Open failure");
	}

	if(host == NULL) WaitNetwork();
}

void WaitNetwork()
{
	TCPsocket connection;
	for(int i = 0; i < 2; i++)
	{
		while (true)
		{
			SDL_Delay(10); //so I don't kill my CPU

			if ((connection = SDLNet_TCP_Accept(sockets[i])))
			{
				sockets[i] = connection;
				break;
			}
		}
	}
}

int Send(void* data) //the data buffer will be used to read from then send
{
	Fighter* fighter = &__entityList[player].data.fighter;
	Transform* trans = &fighter->trans;
	float knockbackX, knockbackY;

	while (true) //the thread will be killed by the game.c main (theoretically)
	{
		*(float*)(buffer[player] + 0) = trans->pos[0];
		*(float*)(buffer[player] + 4) = trans->pos[1];
		*(float*)(buffer[player] + 8) = trans->vel[0];
		*(float*)(buffer[player] + 12) = trans->vel[1];
		*(int*)(buffer[player] + 16) = (int) fighter->fightState; //only needs a byte but w/e
		*(float*)(buffer[player] + 20) = fighter->health;

		knockbackX = fighter->storedKnockback[0]; //I figured this solution would be thread-safer since the entity list is not locked when it is used
		knockbackY = fighter->storedKnockback[1];
		fighter->storedKnockback[0] -= knockbackX; //I think -= is going to be thread unsafe: confirmed http://stackoverflow.com/questions/9278764/are-primitive-datatypes-thread-safe-in-java #3
		fighter->storedKnockback[1] -= knockbackY;
		*(float*)(buffer[player] + 24) = knockbackX;
		*(float*)(buffer[player] + 28) = knockbackY;

		if (SDLNet_TCP_Send(sockets[ player], buffer[ player], 32) < 10) //Primary Reference: http://content.gpwiki.org/index.php/SDL:Tutorial:Using_SDL_net
		{
			printf("(send-error)");	
		}

		SDL_Delay(10); //so I don't kill my CPU or bandwidth
	}

	return 0;
}

int Receive(void* data) //the function will receive and write to the data buffer
{
	Fighter* them = &__entityList[!player].data.fighter;
	Fighter* you = &__entityList[player].data.fighter; //knockback yourself when the enemy hits you

	while (true) //the thread will be killed by the game.c main (theoretically)
	{
		if (SDLNet_TCP_Recv(sockets[!player], buffer[!player], 32) > 0) //Primary Reference: http://content.gpwiki.org/index.php/SDL:Tutorial:Using_SDL_net
		{	
			them->trans.pos[0] = *(float*) (buffer[!player] + 0);
			them->trans.pos[1] = *(float*) (buffer[!player] + 4);
			them->trans.vel[0] = *(float*) (buffer[!player] + 8);
			them->trans.vel[1] = *(float*) (buffer[!player] + 12);
			them->fightState = (FighterState) * (int*) ( buffer[!player] + 16);
			them->health = *(float*) (buffer[!player] + 20);

			you->trans.vel[0] += *(float*) (buffer[!player] + 24);
			you->trans.vel[1] += *(float*) (buffer[!player] + 28);
		}
		else
		{
			printf("(receive-error)");
			SDL_Delay(10);
		}
	}

	return 0;
}

void CloseNetwork()
{
	SDLNet_TCP_Close(sockets[0]); //redundant due to SDLNet_Quit but w/e
	SDLNet_TCP_Close(sockets[1]);

	SDLNet_Quit();
}