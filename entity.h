#ifndef ENTITY_H
#define ENTITY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"
#include "glib.h"
#include "vector2.h"

typedef enum { NONE, PLAYER, PROJECTILE, WALL } Selection;

typedef struct 
{
	Sprite* sprite;
	int frame;
	vec2d pos;
	vec2d vel;
	SDL_Rect BBox;
	float radius;
} Fighter;

typedef struct 
{
	Sprite* sprite;
	int frame;
	vec2d pos;
	vec2d vel;
	SDL_Rect BBox;
	float radius;
} Projectile;

typedef struct 
{
	Sprite* sprite;
	int frame;
	vec2d pos;
	vec2d vel;
	SDL_Rect BBox;
	float radius;
} HealthBar;

typedef struct 
{
	Sprite* sprite;
	float health;
	SDL_Rect BBox;
} Controller;

typedef struct 
{
	Sprite* sprite;
	int frame;
	vec2d pos;
	vec2d vel;
	SDL_Rect BBox;
	float radius;
} Wall;

typedef union Data_S
{	
	// Possible states when it's in use.	
	Fighter fighter;
	Projectile projectile;
	HealthBar healthBar;
	Controller controller;
} Data;

typedef struct Entity_S
{
	int used; //should this object be updated?
	int visible; //should we render the object?

	/* Function Pointers */

	void (*think) (struct Entity_S* self);
	void (*update) (struct Entity_S* self);
	void (*draw) (struct Entity_S* self);

    struct Entity_S* next;
	Data data;
} Entity;

void CloseEntityList();
void DrawEntity(Entity *ent);
void DrawEntityList();
void FreeEntity(Entity** ent);
Entity* GetEntity();
void InitEntityList();
void ThinkEntityList();
void UpdateEntityList();

#endif