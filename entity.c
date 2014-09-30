#include "entity.h"

void CloseEntityList()
{
	int i;
	for(i = 0; i < __MaxEntities; i++) //free every entity in the list
	{
		Entity* p = &__entityList[i]; //this pointer will be cleared by freeEntity, that's why it takes a pointer to a pointer
		FreeEntity(&p); //DAFUQ, that's a pointer to a pointer.
	}
}

void DrawEntityList()
{	
	int i;
	for(i = 0; i < __MaxEntities;i++)
	{
		if(__entityList[i].used)
		{
			if(__entityList[i].visible) __entityList[i].draw(&__entityList[i]);
		}
	}
}

void FreeEntity(Entity** ent)
{
	//handle freeing resources like Sprite Data
	memset(*ent,0,sizeof(Entity));
	*ent = NULL;
}

Entity* GetEntity()
{
	Entity* ent = __firstFree; //remember __firstFree is an Entity pointer already, we don't need to &__firstFree it.

	if(ent != NULL) __firstFree = __firstFree->data.next; //Don't fuck up, read this line until you understand it.

	return ent; //hmmm, I wonder what happens if __firstFree was NULL?

	//RIP DJ's CODE

	/*
	int i;
	for(i = 0; i < __MaxEntities; i++) //CONSIDER: Round-Robin, C'mon DJ
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
	//load entity config from file...

	int i;

	__firstFree = &__entityList[0];

	__entityList = (Entity*) malloc(sizeof(Entity) * __MaxEntities);
	if(__entityList == NULL)
	{
		fprintf(stderr,"initEntityList: FATAL: cannot allocate entity list\n");
		exit(-1);
		return;
	}
	memset(__entityList, 0, sizeof(Entity) * __MaxEntities);

	for(i = 0; i < __MaxEntities - 1; i++) //set the pointer to the next element for every entity ¡EXCEPT! the last
	{
		__entityList[i].data.next = &__entityList[i+1];
	}
	__entityList[__MaxEntities-1].data.next = NULL;
}

void ThinkEntityList()
{
	int i;
	for(i = 0;i < __MaxEntities;i++)
	{
		if(__entityList[i].used)
		{
			if(__entityList[i].think != NULL) __entityList[i].think(&__entityList[i]);
		}
	}
}

void UpdateEntityList()
{
	int i;
	for(i = 0;i < __MaxEntities;i++)
	{
		if(__entityList[i].used)
		{
			if(__entityList[i].update != NULL) __entityList[i].update(&__entityList[i]);
		}
	}
}