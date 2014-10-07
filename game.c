#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"
#include "glib.h"
#include "vector2.h"
#include "entity.h"

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

	Sprite* tile;

	int keyn;
	int i;
	int mx,my;

	Uint8* keys;

	InitAll();

	temp = IMG_Load("images/fire_and_ice.jpg");
	
	if(temp != NULL) bg = SDL_DisplayFormat(temp); // ALWAYS check your pointers before you use them

	SDL_FreeSurface(temp);

	if(bg != NULL) SDL_BlitSurface(bg, NULL, buffer, NULL);

	tile = LoadSprite("images/32_32_16_2sprite.png", 32, 32, 32);

	if(tile != NULL)
	{
		for(i = 0;i < 12;i++)
		{
			DrawSprite(tile, buffer, i * tile->w, 0, 0);
		}
	}

	do
	{
		//Draw Events
		ResetBuffer ();
		DrawEntityList();
		DrawMouse();
		NextFrame();

		//Input and Action Events
		SDL_PumpEvents();
		keys = SDL_GetKeyState(&keyn);
		//ProcessInput();
		//ThinkEntityList();
		//UpdateEntityList();

		if(SDL_GetMouseState(&mx,&my)) DrawSprite(tile, buffer, (mx /32) * 32, (my /32) * 32, 0); 

		if(keys[SDLK_ESCAPE]) quit = 1;

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
	atexit(CleanUpAll);
}

void InitFighters()
{
	InitPlayer("images/hypersphere.png", 200, 600);
	InitPlayer("images/hypersphere.png", 800, 600, Magenta_);
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

void ProcessInput()
{
	SDL_Event event;

	while( SDL_PollEvent( &event ) )
	{
		switch( event.type )
		{
			/* Keyboard event */
			/* Pass the event data onto PrintKeyInfo() */
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			//PrintKeyInfo( &event.key );
			break;

			/* SDL_QUIT event (window close) */
			case SDL_QUIT:
			quit = 1;
			break;

			default:
			break;
		}
	}
}