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

	struct Entity
	{
		int used; //is this object an available resource? (Note: if it is NONE should be set)
		int visible;

		typedef enum Type { NONE, PLAYER, PROJECTILE, WALL };
		Type type; //NOTE: neccessary to check against NONE (keep separate)

		/* Function Pointers */

		void (*think) (Entity* self);
		void (*update) (Entity* self);
		void (*draw) (Entity* self);

		typedef struct Player
		{
			Sprite* sprite;
			int frame;
			vector2 pos;
			vector2 vel;
			SDL_Rect BBox;
			float radius;
		};

		typedef struct Projectile
		{
			Sprite* sprite;
			int frame;
			vector2 pos;
			vector2 vel;
			SDL_Rect BBox;
			float radius;
		};

		typedef struct Wall
		{
			Sprite* sprite;
			int frame;
			vector2 pos;
			vector2 vel;
			SDL_Rect BBox;
			float radius;
		};

		union Data
		{
			// State when it's available.
			Entity* next;
			
			// Possible states when it's in use.	
			Player player;
			Projectile projectile;
			Wall wall;
		} data;
	};

	int __MaxEntities = 512;
	Entity* __entityList;
	Entity* __firstFree;

	void CloseEntityList();
	void DrawEntity(Entity *ent);
	void DrawEntityList();
	void FreeEntity(Entity** ent);
	Entity* GetEntity();
	void InitEntityList();
	void ThinkEntityList();
	void UpdateEntityList();

#endif