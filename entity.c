#include "entity.h"
#include "quadtree.h"

#define __maxEntities 32
#define __entityPermutation 3

void __funcPtrInit();

Entity* __entityList = NULL;
Entity* __firstFree = NULL;

void  (*draw    [__entityPermutation])(void* data); //an array of function pointers to draw functions that take void pointers to the data union type
void  (*destroy [__entityPermutation])(void* data);
void* (*init    [__entityPermutation])(void* data);
void  (*think   [__entityPermutation])(void* data);
void  (*update  [__entityPermutation])(void* data);

void __funcPtrInit()
{
	draw   [0] = NULL; //NONE draw function
	destroy[0] = NULL;
	init   [0] = NULL;
	think  [0] = NULL;
	update [0] = NULL;

	draw   [1] = FighterDraw; //look at all that hardcoding.
	destroy[1] = FighterDestroy;
	init   [1] = FighterInit;
	think  [1] = FighterThink;
	update [1] = FighterUpdate;

	draw   [2] = ProjectileDraw;
	destroy[2] = ProjectileDestroy;
	init   [2] = ProjectileInit;
	think  [2] = ProjectileThink;
	update [2] = ProjectileUpdate;
}

void CloseEntityList()
{
	int i;
	for(i = 0; i < __maxEntities; i++) //free every entity in the list
	{
		Entity* p = &__entityList[i]; //this pointer will be cleared by freeEntity, that's why FreeEntity takes a pointer to a pointer
		FreeEntity(&p); //DAFUQ, that's a pointer to a pointer.
	}
}

void DrawEntityList()
{	
	int i;

	for(i = 0; i < __maxEntities;i++)
	{
		if(__entityList[i].used)
		{
			if(__entityList[i].visible)
			{
				//fprintf(stderr, "Check: /%x/",__entityList[i].data.fighter.sprite);
				void (*func)(void* data) = draw[__entityList[i].entType]; //as long as you have a draw function
				if(func) func(&__entityList[i].data); //draw the entity!
			}
		}
	}
}

void FreeEntity(Entity** ent)
{
	if(ent == NULL || (*ent) == NULL) return;

	void (*func)(void* data) = destroy[(*ent)->entType];
	if(func) func(&(*ent)->data);

	(*ent)->entType = NONE; //ashes to ashes, NONE to NONE
	(*ent)->used = 0;
	(*ent)->visible = 0;
	(*ent)->next = __firstFree; //make sure to add the entity back into the object pool
	__firstFree = *ent;

	*ent = NULL;
}

void* GetEntity(EntityType entType)
{
	Entity* ent = __firstFree; //remember __firstFree is an Entity pointer already, we don't need to &__firstFree it.

	if(__firstFree != NULL) __firstFree = __firstFree->next; //Don't fuck up, read this line until you understand it.

	if(!ent) return NULL; 

	ent->entType = entType; //set type
	ent->used = 1;
	ent->visible = 1;

	void* (*func)(void* data) = init[entType];
	if(func) func(&ent->data);

	return &ent->data;

	//RIP DJ's CODE

	/*
	int i;
	for(i = 0; i < __maxEntities; i++) //CONSIDER: Round-Robin, C'mon DJ
	{
		if(__entityList[i].used == 0)
		{
			__entityList[i].used = 1;
			return &__entityList[i];
		}
	}
	return NULL;
	*/
}

void InitEntityList()
{
	int i;

	//load entity config from file...

	__funcPtrInit();

	__entityList = (Entity*) malloc(sizeof(Entity) * __maxEntities);
	
	if(__entityList == NULL)
	{
		fprintf(stderr,"initEntityList: FATAL: cannot allocate entity list\n");
		exit(-1);
		return;
	}

	__firstFree = &__entityList[0]; //__entityList must be initialized first!

	memset(__entityList, 0, sizeof(Entity) * __maxEntities);

	for(i = 0; i < __maxEntities - 1; i++) //set the pointer to the next element for every entity ¡EXCEPT! the last
	{
		__entityList[i].entType = NONE;
		__entityList[i].next = &__entityList[i+1];
	}

	__entityList[__maxEntities-1].entType = NONE;
	__entityList[__maxEntities-1].next = NULL;
}

void ThinkEntityList()
{
	int i;
	for(i = 0;i < __maxEntities;i++)
	{
		if(__entityList[i].used)
		{
			void (*func)(void* data) = think[__entityList[i].entType];
			if(func) func(&__entityList[i].data);
		}
	}
}

void UpdateEntityList()
{
	int i;
	for(i = 0;i < __maxEntities;i++)
	{
		if(__entityList[i].used)
		{
			void (*func)(void* data) = update[__entityList[i].entType];
			if(func) func(&__entityList[i].data);
		}
	}
}

void PopulateQuadtrees()
{
	Entity* ent;

	PrepareQuadtrees();
	for(int i = 0; i < __maxEntities; i++)
	{
		ent = &__entityList[i];
		if(!ent->used) continue;

		InsertCollider(ent, &ent->rect);
	}
}

void FetchCollisions(Entity* player)
{
	Entity* out[16];
	int i;
	float x, y, mag;
	float x1, y1, r1;
	float x2, y2, r2;

	RetrieveCollisions(&player->rect, (void* (*)[]) &out);

	AABBtoCircle(&player->rect, &x1, &y1, &r1);

	for(i = 0; i < 16; i++)
	{
		if(!out[i]) continue;

		AABBtoCircle(&out[i]->rect, &x2, &y2, &r2);

		if(CircleOnCircle(x1,y1,r1,x2,y2,r2,&x,&y))
		{
			mag = sqrt(x*x + y*y);
		}
	}
}