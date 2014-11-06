#ifndef FONT_H
#define FONT_H

#define F_Small 0
#define F_Medium 1
#define F_Large 2

void LoadFonts();
void DrawTextCentered(char* text,SDL_Surface* surface, int sx, int sy, Uint32 color, int size, int font);

#endif