#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"
#include "glib.h"
#include "vector2.h"

extern SDL_Surface* screen;
extern SDL_Surface* buffer; //pointer to the draw buffer
extern SDL_Rect Camera;

void InitAll();
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
	//temp = IMG_Load("images/bgtest.png"); // notice that the path is part of the filename
	if(temp != NULL) bg = SDL_DisplayFormat(temp); // ALWAYS check your pointers before you use them

	SDL_FreeSurface(temp);

	if(bg != NULL) SDL_BlitSurface(bg, NULL, buffer, NULL);

	tile = LoadSprite("images/32_32_16_2sprite.png", 32, 32);

	if(tile != NULL)
	{
		for(i = 0;i < 12;i++)
		{
			DrawSprite(tile, buffer, i * tile->w, 0, 0);
		}
	}

	do
	{
		ResetBuffer ();
		DrawMouse();
		NextFrame();
		SDL_PumpEvents();
		keys = SDL_GetKeyState(&keyn);
		//ProcessInput();

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
	//any other cleanup functions can be added here
}

/**
 * Conjecture: everything required for game set-up
 */
void InitAll()
{
	InitGraphics();

	InitMouse();
	atexit(CleanUpAll);
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