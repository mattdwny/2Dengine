#include <stdlib.h>
#include <string.h>
#include "SDL_ttf.h"
#include "graphics.h"
#include "globals.h"
#include "font.h"

#define __maxFonts 3

extern SDL_Surface* screen;
TTF_Font* __fonts[__maxFonts];

void LoadFont(char filename[40],int ptsize,int type);

void LoadFonts()
{
	if(TTF_Init() == 0) atexit(TTF_Quit);
	else				CRASH("Couldn't initialize Font System: ", SDL_GetError());

	memset(__fonts,0,sizeof(TTF_Font*) * __maxFonts);
	
	LoadFont("fonts/font.ttf",16,F_Small);
	LoadFont("fonts/font1.ttf",32,F_Medium);
	LoadFont("fonts/font2.ttf",64,F_Large);
}

void LoadFont(char filename[40],int ptsize,int type)
{
	__fonts[type] = TTF_OpenFont(filename,ptsize);
	if(__fonts[type] == NULL) CRASH("Couldn't initialize Font: ", SDL_GetError());
}

void DrawTextCentered(char* text,SDL_Surface* surface, int sx, int sy, Uint32 color, int size, int font)
{
	SDL_Surface *temp1 = NULL;
	SDL_Surface *fontpic = NULL;
	SDL_Color colortype,bgcolor;
	SDL_Rect dst;
	SDL_GetRGBA(color, screen->format, &colortype.r, &colortype.g, &colortype.b, &colortype.unused);
	bgcolor.r = 0;
	bgcolor.g = 0;
	bgcolor.b = 0;
	bgcolor.unused = SDL_ALPHA_TRANSPARENT;

	if(__fonts[font] == NULL) return;
	temp1 = TTF_RenderText_Shaded(__fonts[font], text, colortype, bgcolor);
	
	fontpic = SDL_DisplayFormat(temp1);
	SDL_FreeSurface(temp1); //TODO: cache this shit.
	dst.x = sx - (fontpic->w / 1);
	dst.y = sy;
	SDL_SetColorKey(fontpic, SDL_SRCCOLORKEY , SDL_MapRGBA(screen->format, bgcolor.r,bgcolor.g,bgcolor.b,bgcolor.unused));
	SDL_BlitSurface(fontpic,NULL,surface,&dst);
	SDL_FreeSurface(fontpic);
}