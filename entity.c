#include "entity.h"

#define __maxEntities 512
#define __entityPermutation 2

Entity* __entityList = NULL;
Entity* __firstFree = NULL;

void (*draw    [__entityPermutation])(void* data); //an array of function pointers to draw functions that take void pointers to the data union type
void (*destroy [__entityPermutation])(void* data);
void (*init    [__entityPermutation])(void* data);
void (*think   [__entityPermutation])(void* data);
void (*update  [__entityPermutation])(void* data);

void CloseEntityList()
{
	int i;
	for(i = 0; i < __maxEntities; i++) //free every entity in the list
	{
		Entity* p = &__entityList[i]; //this pointer will be cleared by freeEntity, that's why it takes a pointer to a pointer
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
			void (*func)(void* data) = draw[__entityList[i].entType];
			if(func) func(&__entityList[i].data);
		}
	}
}

void FreeEntity(Entity** ent)
{
	void (*func)(void* data) = destroy[(*ent)->entType];
	if(func) func(&(*ent)->data);

	(*ent)->entType = NONE; //ashes to ashes, NONE to NONE

	(*ent)->next = __firstFree; //make sure to add the entity back into the object pool
	__firstFree = *ent;
}

void* GetEntity(EntityType entType)
{
	Entity* ent = __firstFree; //remember __firstFree is an Entity pointer already, we don't need to &__firstFree it.

	if(__firstFree != NULL) __firstFree = __firstFree->next; //Don't fuck up, read this line until you understand it.

	if(!ent) return NULL; 

	void (*func)(void* data) = init[entType];
	if(func) func(&__entityList[i].data);

	if(ent) return (void*) &ent->data;

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

Projectile* GetProjectile(int player)
{
	Entity* ent = __GetEntity(PROJECTILE);
	if(!ent) return NULL;

	ent->entType = PROJECTILE;

	Projectile* projectile = &ent->data.projectile;

	//

	return projectile;
}

void InitEntityList()
{
	int i;

	//load entity config from file...

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
		__entityList[i].next = &__entityList[i+1];
	}

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
			if(__entityList[i].update != NULL) if(func) func(&__entityList[i].data);
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
			if(__entityList[i].update != NULL) if(func) func(&__entityList[i].data);
		}
	}
}