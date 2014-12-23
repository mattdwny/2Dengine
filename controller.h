#ifndef CONTROLLER_H
#define CONTROLLER_H

#define HLEFT 0
#define HRIGHT 1
#define VUP 0
#define VDOWN 1

typedef enum { AXIS_MOVE_H = 0, AXIS_MOVE_V = 1, AXIS_SMASH_H = 2 /*unused*/, AXIS_SMASH_V = 3 /*unused*/ } Axes_E ;
typedef enum { BUTTON_LIGHT = 0, BUTTON_MED = 1, BUTTON_HEAVY = 2, BUTTON_SPECIAL = 3, BUTTON_JUMP = 4 /*unused*/, BUTTON_GUARD = 5, BUTTON_GRAB = 6 /*unused*/ } Buttons_E;

typedef struct Controller_S
{
	float   axes[4];
	int		__axes[4][2];
	Uint8 buttons[7];
	void (*control)(Controller_S*, Axes_E, int, int);
} Controller;

void OpenControllers(int map);
void ProcessInput();
int  InputThread(void* data);
void ProcessAxis(Controller_S* controller, Axes_E axis, int dir, int press);

#endif