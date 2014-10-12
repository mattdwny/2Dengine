#ifndef CONTROLLER_H
#define CONTROLLER_H

typedef enum Axes {AXIS_MOVE_H, AXIS_MOVE_V, AXIS_SMASH_H, AXIS_SMASH_V};
typedef enum Buttons {BUTTON_ATTACK, BUTTON_SPECIAL, BUTTON_GUARD, BUTTON_GRAB };

int   axes[4];
Uint8 buttons[4];
const Uint8* keyboard;

void InitController();
void ProcessInput();

#endif