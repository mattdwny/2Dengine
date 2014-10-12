#ifndef CONTROLLER_H
#define CONTROLLER_H

typedef enum { AXIS_MOVE_H, AXIS_MOVE_V, AXIS_SMASH_H, AXIS_SMASH_V } Axes1 ;
typedef enum { BUTTON_ATTACK, BUTTON_SPECIAL, BUTTON_JUMP, BUTTON_GUARD, BUTTON_GRAB } Buttons1;

void InitController();
void ProcessInput();

#endif