#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"
#include "glib.h"
#include "vector2.h"
#include "entity.h"
#include "controller.h"

extern SDL_Surface* screen;
extern SDL_Surface* buffer; //pointer to the draw buffer
extern SDL_Rect Camera;

void InitAll();
void InitFighters();
void InitPlayer(char* str, float x, float y);
void InitPlayer(char* str, float x, float y, int c);
void CleanupAll();
void ProcessInput();

int quit = 0;

/**
 * this program must be run from the directory directly below images and src, not from within src
 * notice the default arguments for main.  SDL expects main to look like that, so don't change it
 */
int main(int argc, char* argv[])
{
	SDL_Surface* temp;
	SDL_Surface* bg;

	InitAll();

	temp = IMG_Load("images/fire_and_ice.jpg");
	
	if(temp != NULL) bg = SDL_DisplayFormat(temp); // ALWAYS check your pointers before you use them

	SDL_FreeSurface(temp);

	if(bg != NULL) SDL_BlitSurface(bg, NULL, buffer, NULL);

	do
	{
		//Draw Events
		ResetBuffer ();
		DrawEntityList();
		DrawMouse();
		NextFrame();

		//Input and Action Events
		SDL_PumpEvents();
		ProcessInput();
		//ThinkEntityList();
		//UpdateEntityList();

	} while(!quit);

	exit(0);   //technically this will end the program, but the compiler likes all functions that can return a value TO return a value

	return 0;
}

/**
 * Conjecture: everything required for game clean-up, saving needs to go here
 */
void CleanUpAll()
{
	CloseSprites();
	CloseEntityList();
	//any other cleanup functions can be added here
}

/**
 * Conjecture: everything required for game set-up
 */
void InitAll()
{
	InitGraphics();
	InitEntityList();
	InitFighters();
	InitMouse();
	//InitController();
	atexit(CleanUpAll);
}

void InitFighters()
{
	InitPlayer("images/hypersphere.png", 200, 600);
	InitPlayer("images/hypersphere2.png", 800, 600, Magenta_);
}

void InitPlayer(char* str, float x, float y)
{
	InitPlayer(str, x, y, -1);
}
void InitPlayer(char* str, float x, float y, int c)
{
	Fighter* fighter = (Fighter*) GetEntity(FIGHTER);

	//fprintf(stderr,"Worked I guess");

	fighter->sprite = LoadSprite(str, 64, 64, 192, c, c, c);
	fighter->frame = 0;
	fighter->pos[0] = x;
	fighter->pos[1] = y;
}