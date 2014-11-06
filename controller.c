#include "SDL.h"
//#include "game.h"
#include "controller.h"

#define MIN(A,B) (A < B ? A : B)

Controller controllers[4]; //hardcoding, BLEH

const Uint8* keyboard;

void HandleKeysDown(SDLKey k);
void HandleKeysUp(SDLKey k);

void InitControllers()
{
	/*int i;
	int numJoysticks;
	
	SDL_Init(SDL_INIT_JOYSTICK);
	atexit(SDL_Quit);

	numJoysticks = SDL_NumJoysticks();

	for(i = 0; i < MIN(numJoysticks,2); ++i)
	{
		SDL_Joystick* js = SDL_JoystickOpen(i);
		if (js)
		{
			int num_axes = SDL_JoystickNumAxes(js);
			int num_buttons = SDL_JoystickNumButtons(js);
			int num_hats = SDL_JoystickNumHats(js);
			int num_balls = SDL_JoystickNumBalls(js);

			printf("#%d:\t axes:%d buttons:%d hats:%d balls:%d\n", i, num_axes, num_buttons, num_hats, num_balls);

			SDL_JoystickClose(js);
		}
	}*/
}

void ProcessInput()
{
	SDL_PumpEvents();
	keyboard = SDL_GetKeyState( NULL );
	SDL_Event event;

	while( SDL_PollEvent( &event ) )
	{
		switch( event.type )
		{
			case SDL_KEYDOWN:
				HandleKeysDown(event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				HandleKeysUp(event.key.keysym.sym);
				break;

			default:
				break;
		}
	}
}

void HandleKeysDown(SDLKey k)
{
	switch(k)
	{
		//controllers[0] movement
		case SDLK_ESCAPE:
			exit(-1);
			break;

		case SDLK_w:
			controllers[0].axes[AXIS_MOVE_V] = -1;
			break;

		case SDLK_a:
			controllers[0].axes[AXIS_MOVE_H] = -1;
			printf("Yo, BR-R-R-O, you pressed the A key, bro.\n");
			break;

		case SDLK_s:
			controllers[0].axes[AXIS_MOVE_V] =  1;
			break;

		case SDLK_d:
			controllers[0].axes[AXIS_MOVE_H] =  1;
			break;

		//controllers[1] movement
		case SDLK_UP:
			controllers[1].axes[AXIS_MOVE_V] = -1;
			break;

		case SDLK_LEFT:
			controllers[1].axes[AXIS_MOVE_H] = -1;
			break;

		case SDLK_DOWN:
			controllers[1].axes[AXIS_MOVE_V] =  1;
			break;

		case SDLK_RIGHT:
			controllers[1].axes[AXIS_MOVE_H] =  1;
			break;
	}
}
void HandleKeysUp(SDLKey k)
{
	switch(k)
	{
		//controllers[0] movement
		case SDLK_ESCAPE:
			//CRASH("DAFUQ");
			break;

		case SDLK_w:
			controllers[0].axes[AXIS_MOVE_V] = 0;
			break;

		case SDLK_a:
			controllers[0].axes[AXIS_MOVE_H] = 0;
			printf("Yo, BR-R-R-O, you released the A key, bro.\n");
			break;

		case SDLK_s:
			controllers[0].axes[AXIS_MOVE_V] = 0;
			break;

		case SDLK_d:
			controllers[0].axes[AXIS_MOVE_H] = 0;
			break;

		//controllers[1] movement
		case SDLK_UP:
			controllers[1].axes[AXIS_MOVE_V] = 0;
			break;

		case SDLK_LEFT:
			controllers[1].axes[AXIS_MOVE_H] = 0;
			break;

		case SDLK_DOWN:
			controllers[1].axes[AXIS_MOVE_V] = 0;
			break;

		case SDLK_RIGHT:
			controllers[1].axes[AXIS_MOVE_H] = 0;
			break;
	}
}