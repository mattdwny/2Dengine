#include "SDL.h"
#include "controller.h"

#define MIN(A,B) (A < B ? A : B)

int   axes[4];
Uint8 buttons[5];
const Uint8* keyboard;



/*void InitController()
{
	int i;
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
	}
}*/

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
				if(event.key.keysym.sym == SDLK_ESCAPE) exit(-1);
				break;
			case SDL_KEYUP:
				
				break;

			default:
				break;
		}
	}
}