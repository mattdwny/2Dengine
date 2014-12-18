#include "SDL.h"
#include "sdl_net.h"
#include "globals.h"
#include "entity.h"

extern Entity* __entityList;

IPaddress addresses[2];
TCPsocket sockets[2];
char buffer[2][256]; //used for SENDER and RECEIVER
int player = 0;

//NOTE: I may have to recreate the code so that client and server are not mixed into the same code "network.c" but hopefully I don't to make using the network more abstract.
//Supplemental resources:
//
//Primary Reference: http://content.gpwiki.org/index.php/SDL:Tutorial:Using_SDL_net
//Best Network Function Documentation: http://www.libsdl.org/projects/SDL_net/docs/SDL_net.pdf
//Multithreading 1: http://lazyfoo.net/tutorials/SDL/46_multithreading/index.php
//Multithreading 2: http://lazyfoo.net/SDL_tutorials/lesson33/
//Multithreading Documentation: http://www.libsdl.org/release/SDL-1.2.15/docs/html/thread.html

/**
 *  @param host: if NULL the function will create a server, otherwise attempt to connect to "host" as a client
 */
void OpenNetwork(const char* host)
{
	if(SDLNet_Init() == -1) CRASH("SDLNet_Init: ", SDLNet_GetError());

	if(host == NULL) player = 0;
	else			 player = 1;

	if(SDLNet_ResolveHost(&addresses[ player], host, 10000) == -1) CRASH("SDLNet_ResolveHost: ", SDLNet_GetError()); //notice the port numbers are different for SEND/RECEIVE
	if(SDLNet_ResolveHost(&addresses[!player], host, 10001) == -1) CRASH("SDLNet_ResolveHost: ", SDLNet_GetError()); 

	sockets[ player] = SDLNet_TCP_Open(&addresses[ player]);	sockets[!player] = SDLNet_TCP_Open(&addresses[!player]);
	
	if(!sockets[ player]) CRASH("SDLNet_TCP_Open SENDER failure");
	if(!sockets[!player]) CRASH("SDLNet_TCP_Open RECEIVER failure");

	//TODO: according to this code the client will connect to the server, but the server will never save the client's connection
}

void WaitNetwork()
{
	TCPsocket connection;
	for(int i = 0; i < 2; i++)
	{
		while (true)
		{
			printf("Hello");

			SDL_Delay(1); //so I don't kill my CPU

			if ((connection = SDLNet_TCP_Accept(sockets[i])))
			{
				SDLNet_TCP_Close(sockets[i]);
				sockets[i] = connection;
				break;
			}
		}
	}
}

void Thread(TCPsocket socket, void (*subroutine)())
{
	while (true) //the thread will be killed by the game.c main (theoretically)
	{
		SDL_Delay(1); //so I don't kill my CPU

		subroutine();
	}
}

void Send() //the data buffer will be used to read from then send
{
	Fighter* fighter = &__entityList[player].data.fighter;

	*(float*)(&buffer[player] + 0) = fighter->pos[0];
	*(float*)(&buffer[player] + 4) = fighter->pos[1];
	*(float*)(&buffer[player] + 8) = fighter->vel[0];
	*(float*)(&buffer[player] + 12) = fighter->vel[1];
	*(FighterState*)(&buffer[player] + 16) = fighter->fightState;
	printf("send: %s/",buffer[player]);
	//Primary Reference: http://content.gpwiki.org/index.php/SDL:Tutorial:Using_SDL_net
	if (SDLNet_TCP_Send(sockets[ player], buffer[ player], 256) > 0)
	{
		
	}
}

void Receive() //the function will receive and write to the data buffer
{
	Fighter* fighter = &__entityList[!player].data.fighter;

	//Primary Reference: http://content.gpwiki.org/index.php/SDL:Tutorial:Using_SDL_net
	if (SDLNet_TCP_Recv(sockets[!player], buffer[!player], 256) > 0)
	{
		fighter->pos[0] = *(float*) (buffer[!player] + 0);
		fighter->pos[1] = *(float*) (buffer[!player] + 4);
		fighter->vel[0] = *(float*) (buffer[!player] + 8);
		fighter->vel[1] = *(float*) (buffer[!player] + 12);
		fighter->fightState = *(FighterState*) (buffer[!player] + 16);
		printf("receive: %s/",buffer[!player]);
	}
}

int SendThread(void*)	 { Thread(sockets[ player],   Send ); }
int ReceiveThread(void*) { Thread(sockets[!player], Receive); }

void CloseNetwork()
{
	SDLNet_TCP_Close(sockets[ player]); //redundant due to SDLNet_Quit but w/e
	SDLNet_TCP_Close(sockets[!player]);

	//There is no inverse to ResolveHost, which just gets an IP address

	SDLNet_Quit();
}