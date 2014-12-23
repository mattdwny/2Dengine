#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_thread.h"
#include "graphics.h"
#include "glib.h"
#include "vector2.h"
#include "entity.h"
#include "controller.h"
#include "font.h"
#include "audio.h"
#include "network.h"

extern SDL_Surface* screen;
extern SDL_Surface* buffer; //pointer to the draw buffer
extern SDL_Rect Camera;

void InitAll(char* host);
void InitControllers(int map);
void InitFighters();
void InitNetwork(char* host);
void InitPlayer(char* str, float x, float y);
void InitPlayer(char* str, float x, float y, int c);
void CleanupAll();
void ProcessInput();
void CountDown(char* string, Uint32 color);

int quit = 0;
SDL_Thread* threads[3] = { NULL, NULL, NULL };

/**
 * this program must be run from the directory directly below images and src, not from within src
 * notice the default arguments for main.  SDL expects main to look like that, so don't change it
 */
int main(int argc, char** argv)
{
	SDL_Surface* temp;
	SDL_Surface* bg;
	int frame = 0;
	int time = 9999;
	char timechar[2];

	if(argc == 1) InitAll(NULL);
	else		  InitAll(argv[1]);

	temp = IMG_Load("images/fire_and_ice.jpg");
	
	if(temp != NULL) bg = SDL_DisplayFormat(temp); // ALWAYS check your pointers before you use them

	SDL_FreeSurface(temp);

	if(bg != NULL) SDL_BlitSurface(bg, NULL, buffer, NULL);

	/*CountDown(" ", Red_);
	CountDown(" ", Red_);
	CountDown(" ", Red_);
	PlaySound(THREE_);
	FrameDelay(200);
	CountDown("3", Red_);
	PlaySound(TWO_);
	CountDown("2", Orange_);
	PlaySound(ONE_);
	CountDown("1", Yellow_);
	PlaySound(GO_);*/

	PlayMusic(BATTLEFIELD);
	//PlayMusic(FINALDESTINATION);
	while(!quit)
	{
		//BEGIN Draw Events
		ResetBuffer ();
		DrawEntityList();
 
		DrawTextCentered(itoa(time/100,timechar,10),screen, ResolutionX_/2 + 24, ResolutionY_/2 - 300, Green_, 0x1337babe, F_Large);
		if(frame++ < 500) DrawTextCentered("GO!",screen, ResolutionX_/2 + 60, ResolutionY_/2 - 200, Green_, 0x1337babe, F_Large);

		NextFrame();
		//END Draw Events

		ProcessInput();
		//fprintf(stderr, "%x\n", green);
		//Input and Action Events
		ThinkEntityList();
		PopulateQuadtrees();
		UpdateEntityList();
		//End Input and Action Events

		time--;
		if(time < 0) goto timeout;
	}

	exit(0);   //technically this will end the program, but the compiler likes all functions that can return a value TO return a value

	timeout:

	PlaySound(TIME_);
	CountDown("Time!", Blue_);
	CountDown("Time!", Blue_);
	CountDown("Time!", Blue_);
	CountDown("Time!", Blue_);
	CountDown("Time!", Blue_);

	wincond:

	PlaySound(GAME_);
	CountDown("Game!", Red_);
	CountDown("Game!", Red_);
	CountDown("Game!", Red_);
	CountDown("Game!", Red_);
	CountDown("Game!", Red_);

	exit(0);

	return 0;
}

/**
 * Everything required for game clean-up, saving needs to go here
 */
void CleanUpAll()
{
	int i;
	for(i = 0; i < 3; i++) SDL_KillThread( threads[i] );

	CloseSprites();
	CloseEntityList();
}

/**
 * Everything required for game set-up
 */
void InitAll(char* host)
{
	InitGraphics();
	LoadFonts();
	InitAudio();
	LoadSounds();
	InitEntityList();
	InitFighters();
	if(!host)
	{
		printf("case 1");
		InitControllers(3);
		//InitNetwork(host);
	}
	else if(tolower(*host) == 104 || tolower(*host) == 108 /*ascii h for host or l for localhost*/)
	{
		printf("case 2");
		InitControllers(1);
		InitNetwork(NULL);
	}
	else
	{
		printf("case 3");
		InitControllers(2);
		InitNetwork(host);
	}
	//InitController();
	atexit(CleanUpAll);
}

void InitControllers(int map)
{
	OpenControllers(map);
	//threads[0] = SDL_CreateThread( InputThread,    "" );
}

void InitFighters()
{
	InitPlayer("images/hypersphere.png", 200, 736);
	InitPlayer("images/hypersphere2.png", 800, 736);
}

void InitNetwork(char* host)
{
	OpenNetwork(host);
	if(host == NULL) WaitNetwork();
	threads[1] = SDL_CreateThread( Send,    "" );
	threads[2] = SDL_CreateThread( Receive, "" );
}

void InitPlayer(char* str, float x, float y)
{
	InitPlayer(str, x, y, -1);
}

void InitPlayer(char* str, float x, float y, int c)
{
	Fighter* fighter = (Fighter*) GetEntity(FIGHTER);
	Transform* trans = &fighter->trans;
	//fprintf(stderr,"Worked I guess");

	fighter->sprite = LoadSprite(str, 64, 64, 192, c, c, c);
	fighter->frame = 0;
	trans->pos[0] = x;
	trans->pos[1] = y;
}

void CountDown(char* string, Uint32 color)
{
	ResetBuffer ();
	DrawEntityList();
 
	DrawTextCentered(string,screen, ResolutionX_/2 + 7, ResolutionY_/2 - 200, color, 0x1337babe, F_Large);

	NextFrame();
	FrameDelay(1000);
}