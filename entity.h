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
#include "fighter.h"
#include "projectile.h"

typedef enum { NONE = 0, FIGHTER = 1, PROJECTILE = 2 } EntityType;

typedef union Data_S
{	
	// Possible states when it's in use.	
	Transform trans; //"inherited" by fighter and projectile, must be first in their declaration or this won't work
	Fighter fighter;
	Projectile projectile;
} Data;

typedef struct Entity_S
{
	int used; //should this object be updated?
	int visible; //should we render the object?

	EntityType entType;
	Transform trans;

    struct Entity_S* next;
	Data data;
} Entity;

void  CloseEntityList();
void  DrawEntityList();
void  FreeEntity(Entity** ent);
void* GetEntity(EntityType entType);
void  InitEntityList();
void  ThinkEntityList();
void  PopulateQuadtrees();
void  UpdateEntityList();

#endif