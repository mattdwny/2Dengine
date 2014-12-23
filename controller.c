#include "SDL.h"
//#include "game.h"
#include "controller.h"

#define MIN(A,B) (A < B ? A : B)

Controller controllers[4]; //hardcoding, BLEH

const Uint8* keyboard;

int bitmap;

void HandleKeysDown0(SDLKey k);
void HandleKeysDown1(SDLKey k);
void HandleKeysUp0(SDLKey k);
void HandleKeysUp1(SDLKey k);

void OpenControllers(int map)
{
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	bitmap = map;
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

int InputThread(void* data)
{
	while(1)
	{
		SDL_Delay(1);

		ProcessInput();
	}
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
				if(bitmap & (1 << 0)) HandleKeysDown0(event.key.keysym.sym);
				if(bitmap & (1 << 1)) HandleKeysDown1(event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				if(bitmap & (1 << 0)) HandleKeysUp0(event.key.keysym.sym);
				if(bitmap & (1 << 1)) HandleKeysUp1(event.key.keysym.sym);
				break;

			default:
				break;
		}
	}
}

void HandleKeysDown0(SDLKey k)
{
	switch(k)
	{
		case SDLK_ESCAPE:
			exit(-1);
			break;

		//controllers[0] movement
		case SDLK_w:
			ProcessAxis(&controllers[0], AXIS_MOVE_V, VUP, 1);
			break;

		case SDLK_a:
			ProcessAxis(&controllers[0], AXIS_MOVE_H, HLEFT, 1);
			break;

		case SDLK_s:
			ProcessAxis(&controllers[0], AXIS_MOVE_V, VDOWN, 1);
			break;

		case SDLK_d:
			ProcessAxis(&controllers[0], AXIS_MOVE_H, HRIGHT, 1);
			break;

		case SDLK_LSHIFT:
			controllers[0].buttons[BUTTON_GUARD] = 1;
			break;

		case SDLK_u:
			controllers[1].buttons[BUTTON_LIGHT] = 1;
			break;

		case SDLK_i:
			controllers[1].buttons[BUTTON_MED] = 1;
			break;

		case SDLK_o:
			controllers[1].buttons[BUTTON_HEAVY] = 1;
			break;

		case SDLK_p:
			controllers[1].buttons[BUTTON_SPECIAL] = 1;
			break;
	}
}

void HandleKeysDown1(SDLKey k)
{
	switch(k)
	{
		case SDLK_ESCAPE:
			exit(-1);
			break;

		//controllers[1] movement
		case SDLK_UP:
			ProcessAxis(&controllers[1], AXIS_MOVE_V, VUP, 1);
			break;

		case SDLK_LEFT:
			ProcessAxis(&controllers[1], AXIS_MOVE_H, HLEFT, 1);
			break;

		case SDLK_DOWN:
			ProcessAxis(&controllers[1], AXIS_MOVE_V, VDOWN, 1);
			break;

		case SDLK_RIGHT:
			ProcessAxis(&controllers[1], AXIS_MOVE_H, HRIGHT, 1);
			break;

		/*case SDLK_RSHIFT:
			controllers[1].buttons[BUTTON_GUARD] = 1;
			break;*/

		case SDLK_0:
			controllers[1].buttons[BUTTON_GUARD] = 1;
			break;

		case SDLK_n:
			controllers[1].buttons[BUTTON_LIGHT] = 1;
			break;

		case SDLK_m:
			controllers[1].buttons[BUTTON_MED] = 1;
			break;

		case SDLK_COMMA:
			controllers[1].buttons[BUTTON_HEAVY] = 1;
			break;

		case SDLK_PERIOD:
			controllers[1].buttons[BUTTON_SPECIAL] = 1;
			break;
	}
}
void HandleKeysUp0(SDLKey k)
{
	switch(k)
	{
		case SDLK_ESCAPE:
			//CRASH("DAFUQ");
			break;


		//controllers[0] movement
		case SDLK_w:
			ProcessAxis(&controllers[0], AXIS_MOVE_V, VUP, 0);
			break;

		case SDLK_a:
			ProcessAxis(&controllers[0], AXIS_MOVE_H, HLEFT, 0);
			break;

		case SDLK_s:
			ProcessAxis(&controllers[0], AXIS_MOVE_V, VDOWN, 0);
			break;

		case SDLK_d:
			ProcessAxis(&controllers[0], AXIS_MOVE_H, HRIGHT, 0);
			break;

		case SDLK_LSHIFT:
			controllers[0].buttons[BUTTON_GUARD] = 0;
			break;
	}
}
void HandleKeysUp1(SDLKey k)
{
	switch(k)
	{
		case SDLK_ESCAPE:
			//CRASH("DAFUQ");
			break;


		//controllers[1] movement
		case SDLK_UP:
			ProcessAxis(&controllers[1], AXIS_MOVE_V, VUP, 0);
			break;

		case SDLK_LEFT:
			ProcessAxis(&controllers[1], AXIS_MOVE_H, HLEFT, 0);
			break;

		case SDLK_DOWN:
			ProcessAxis(&controllers[1], AXIS_MOVE_V, VDOWN, 0);
			break;

		case SDLK_RIGHT:
			ProcessAxis(&controllers[1], AXIS_MOVE_H, HRIGHT, 0);
			break;

		case SDLK_RSHIFT:
			controllers[1].buttons[BUTTON_GUARD] = 0;
			break;
	}
}


void ProcessAxis(Controller_S* controller, Axes_E axis, int dir, int press)
{
	controller->__axes[axis][dir] = press;
	if (press)								 controller->axes[axis] = (dir ?  1.0f : -1.0f); //I couldn't think of an elegant way to write this, but it does input "snap" similarly to Unity Engine
	else if (controller->__axes[axis][!dir]) controller->axes[axis] = (dir ? -1.0f :  1.0f);
	else									 controller->axes[axis] = 0;
}