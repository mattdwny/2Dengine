#ifndef CONTROLLER_H
#define CONTROLLER_H

typedef enum { AXIS_MOVE_H = 0, AXIS_MOVE_V = 1, AXIS_SMASH_H = 2, AXIS_SMASH_V = 3 } Axes1 ;
typedef enum { BUTTON_ATTACK = 0, BUTTON_SPECIAL = 1, BUTTON_JUMP = 2, BUTTON_GUARD = 3, BUTTON_GRAB = 4 } Buttons1;

void InitController();
void ProcessInput();

#endif