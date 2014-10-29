#ifndef GRAPHICS_H
#define GRAPHICS_H

/**
 *	graphics.h
 *	Originaly written by Donald Kehoe for the NJIT Introduction to game development class
 *	This file contains data structures, constants and function prototypes needed to initialize SDL and
 *	to handle sprites and graphics.
 */

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#define ResolutionX_ 1024
#define ResolutionY_ 768

/*uint32*/
#define Red_         0xFFDD0000 //AAAAHHHH this is not including the implicit A at the front!!!! likely set to zero!
#define Green_       0xFF00DD00
#define Blue_        0xFF0000DD
#define Yellow_      0xFFDDDD00
#define Orange_      0xFFEE5522
#define Violet_      0xFFDD00DD
#define Brown_       0xFF663311
#define Grey_        0xFF888888
#define DarkRed_     0xFF880000
#define DarkGreen_   0xFF008800
#define DarkBlue_    0xFF000088
#define DarkYellow_  0xFF888800
#define DarkOrange_  0xFF774411
#define DarkViolet_  0xFF660066
#define DarkBrown_   0xFF442205
#define DarkGrey_    0xFF444444
#define LightRed_    0xFFFF2222
#define LightGreen_  0xFF33FF33
#define LightBlue_   0xFF3333FF
#define LightYellow_ 0xFFFFFF11
#define LightOrange_ 0xFFFFDD11
#define LightViolet_ 0xFFFF55FF
#define LightBrown_  0xFF886622
#define LightGrey_   0xFFBBBBBB
#define Black_       0xFF000000
#define White_       0xFFFEFEFE
#define Tan_         0xFFFFDD22
#define Gold_        0xFFBB9900
#define Silver_      0xFFAAAABB
#define YellowGreen_ 0xFF44EE01
#define Cyan_        0xFF00FFFF
#define Magenta_     0xFFFF00FF

/**
 * color constants:
 */
enum colors {Red = 1,Green = 2,Blue = 3,Yellow = 4,Orange = 5,Violet = 6,Brown = 7,Grey = 8,DarkRed = 9,DarkGreen = 10,
               DarkBlue = 11,DarkYellow = 12,DarkOrange = 13,DarkViolet = 14,DarkBrown = 15,DarkGrey = 16,LightRed =17,
               LightGreen = 18,LightBlue = 19,LightYellow = 20,LightOrange = 21,LightViolet = 22,LightBrown = 23,LightGrey = 24,
               Black = 25,White = 26,Tan = 27,Gold = 28,Silver = 29,YellowGreen = 30,Cyan = 31,Magenta = 32};

typedef struct
{
	SDL_Surface* image;			//pointer to the actual image in memory
	char filename[64];			//the name of the file, keep them short
	int w, h;					//the width and height of the frames of the sprites, not the file itself
	int framesperline;			//default is 16
	int frames;
	int color1, color2, color3;	//if the file was palette swapped these are the colors that were changed
	int used;					//used by the maintanenc functions to keep track of how many times a single sprite is being used
} Sprite;

typedef struct	//this didn't need to be its own type, its only used once
{
	int xres, yres, depth;
} ScreenData;

void InitGraphics();
void DrawPixel(SDL_Surface* screen, Uint8 R, Uint8 G, Uint8 B, int x, int y);
void BlankScreen(SDL_Surface* buf, Uint32 color);
void SwapSprite(SDL_Surface* sprite, int color1, int color2, int color3);
void InitSpriteList();
void FreeSprite(Sprite* img);
Sprite* LoadSprite(char* filename, int sizex, int sizey, int frames, int c1, int c2, int c3);		//simplest file to load
Sprite* LoadSprite(char* filename, int sizex, int sizey, int frames);

/**
 * drawsprite and drawspritepart work the same except that Drawsprite will render an entire sprite on the desired surface
 * and drawspritepart can have the size of the sprite specified.  Know your sprite when using DSP since you can in theory
 * read too far, not that SDL will allow that.
 */
void DrawSprite(Sprite *sprite,int sx,int sy, int frame);
void DrawSprite(Sprite *sprite,SDL_Surface *surface,int sx,int sy, int frame);

/**
 * call this before you exit to make sure all the memory that your program used is given back to your OS
 */
void CloseSprites();

Uint32 IndexColor(int color);
void FrameDelay(Uint32 delay);
void ResetBuffer();
void NextFrame();
void InitMouse();
void DrawMouse();
void fill_circle(SDL_Surface *surface, int n_cx, int n_cy, int radius, Uint32 pixel);
Uint32 colorLerp(Uint32 color1, Uint32 color2, float t);

#endif
