#include "SDL.h"
#include "controller.h"

#define MIN(A,B) (A < B ? A : B)

void InitController()
{
	SDL_Init(SDL_INIT_JOYSTICK);
	atexit(SDL_Quit);

	int num_joysticks = SDL_NumJoysticks();
	int i;
	for(i = 0; i < MIN(num_joysticks,2); ++i)
	{
		SDL_Joystick* js = SDL_JoystickOpen(i);
		if (js)
		{
			int num_axes = SDL_JoystickNumAxes(js);
			int num_buttons = SDL_JoystickNumButtons(js);
			int num_hats = SDL_JoystickNumHats(js);
			int num_balls = SDL_JoystickNumBalls(js);

			printf("%d:\t axes:%d buttons:%d hats:%d balls:%d\n", i,
					num_axes, num_buttons, num_hats, num_balls);

			SDL_JoystickClose(js);
		}
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
			/* Keyboard event */
			/* Pass the event data onto PrintKeyInfo() */
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			//PrintKeyInfo( &event.key );
			break;

			/* SDL_QUIT event (window close) */
			case SDL_QUIT:
				exit(1);
			break;

			default:
			break;
		}
	}
}