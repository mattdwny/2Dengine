#ifndef GLOBALS_H
#define GLOBALS_H

#define ResolutionX_ 1024
#define ResolutionY_ 768
#define ResolutionXY_ (1024*768)

//I learned my lesson! no definitions in headers to avoid multiply defined objects due to the complicated nature of linking. Got it.
void CRASH(char* msg); /*a simple implementation to make the code more concise. Turns four lines into one.*/
void CRASH(char* msg, char* err); /*an overloaded crash method for SDL error printing*/

#endif