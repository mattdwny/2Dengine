#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "quadtree.h"

#define __maxDepth 3 /* the max number of quadtrees dereferences to get to target quadtree from root */
#define __targetSize 5 /* the preferred number of objects that should reside within the quadtree */
#define __maxQuadtrees (1 + 4 + 4*4 + 4*4*4) /* depth of 0, 1, 2, 3 respectively */

Quadtree* __quadtreeList = NULL; /** the list of quadtrees */
Quadtree* __firstFreeQuadtree = NULL; /** the first available quadtree in the quadtree list */

void InitQuadtrees()
{
	int i;

	__quadtreeList = (Quadtree*) malloc(sizeof(Quadtree) * __maxQuadtrees); /* allocate the required memory to hold all the quadtrees */

	if(__quadtreeList == NULL)
	{
		fprintf(stderr,"InitQuadtrees: FATAL: cannot allocate entity list\n");
		exit(-1);
		return;
	}

	memset(__quadtreeList,0,sizeof(Quadtree) * __maxQuadtrees); /* make sure the memory is wiped before using it */

	__firstFreeQuadtree = &__quadtreeList[0]; /* __quadtreeList must be initialized first! */

	for(i = 0; i < __maxQuadtrees - 1; i++) /* set the pointer to the next element for every entity ¡EXCEPT! the last */
	{
		__quadtreeList[i].next = &__quadtreeList[i+1];
	}

	__quadtreeList[__maxQuadtrees-1].next = NULL; /* last element has no next */
}

Quadtree* GetQuadtree(int depth, float minWidth, float maxWidth, float minHeight, float maxHeight)
{
	if(__firstFreeQuadtree == NULL) return NULL;

	Quadtree* quad = __firstFreeQuadtree;

	__firstFreeQuadtree = __firstFreeQuadtree->next;

	quad->depth = depth;

	quad->bounds.min[0] = minWidth;
	quad->bounds.min[1] = minHeight;
	quad->bounds.max[0] = maxWidth;
	quad->bounds.max[1] = maxHeight;

	return quad;
}

/**
 * Clears the quadtree and all childed quadtrees in submatrix
 */
void FreeQuadtree(Quadtree* quad)
{
	for (int i = 0; i < 4; i++)
	{
		if (quad->submatrix[i] != NULL) FreeQuadtree(quad->submatrix[i]);
	}
	memset(quad,0,sizeof(Quadtree)); /* set all information to NULL to avoid errors later */

	quad->next = __firstFreeQuadtree; /* set next so the current quadtree free pointer list can be reused */
	__firstFreeQuadtree = quad; /* and make the current element the head of the free pointer list */
}

/*
 * Splits the node into 4 subnodes
 */
void SplitQuadtree(Quadtree* quad)
{
	int nextDepth = quad->depth + 1;

	float minWidth   = floor(quad->bounds.min[0]);
	float minHeight  = floor(quad->bounds.min[1]);
	float maxWidth   = ceil(quad->bounds.max[0] + 1);
	float maxHeight  = ceil(quad->bounds.max[1] + 1);
	float halfWidth  = (minWidth  + maxWidth ) / 2;
	float halfHeight = (minHeight + maxHeight) / 2;
 
	quad->submatrix[0] = GetQuadtree(nextDepth, minWidth, halfWidth, minHeight, halfHeight); /* Create all four quadrant permutations */
	quad->submatrix[1] = GetQuadtree(nextDepth, halfWidth, maxWidth, minHeight, halfHeight);
	quad->submatrix[2] = GetQuadtree(nextDepth, minWidth, halfWidth, halfHeight, maxHeight);
	quad->submatrix[3] = GetQuadtree(nextDepth, halfWidth, maxWidth, halfHeight, maxHeight);
}

/*
 * Determine which node the object belongs to. -1 means
 * object cannot completely fit within a child node and is part
 * of the parent node
 */
int GetQuadtreeIndex(AABB* rect, Quadtree* quad)
{
	int index = -1;
	double halfX = ( floor(quad->bounds.min[0]) + ceil(quad->bounds.max[0]) ) / 2;
	double halfY = ( floor(quad->bounds.min[1]) + ceil(quad->bounds.max[1]) ) / 2;
 
	int topQuadrant;
	int bottomQuadrant;

	// Object can completely fit within the top quadrants
	topQuadrant = (rect->min[1] >= halfY);

	// Object can completely fit within the bottom quadrants
	bottomQuadrant = (rect->max[1] < halfY);
 
	if (rect->max[0] < halfX) /* Object can completely fit within the left quadrants */
	{
		if      (topQuadrant)    index = 0;
		else if (bottomQuadrant) index = 2;
	}
	else if (rect->min[0] >= halfX) /* Object can completely fit within the right quadrants */
	{
		if      (topQuadrant)    index = 1;
		else if (bottomQuadrant) index = 3;
	}
 
	return index;
}

/*
 * Insert the object into the quadtree. If the node
 * exceeds the capacity, it will split and add all
 * objects to their corresponding nodes.
 */
 void InsertCollider(Entity* ent, Quadtree* quad)
 {
	int i;
	int objects;
	int index;

	if (quad->submatrix[0] != NULL) /* either all quadtrees in submatrix are null or none are */
	{
		index = GetQuadtreeIndex(&ent->rect, quad);
 
		if (index != -1)
		{
			InsertCollider(ent, quad->submatrix[index]);
			return;
		}
	}

	objects = quad->objects;
	
	if(objects >= 21)
	{
		fprintf(stderr, "FATAL ERROR: Too many objects in one quadtree\n");
		exit(-1);
	}

	quad->list[objects] = ent;

	if (objects > __targetSize && quad->depth < __maxDepth)
	{
		if (quad->submatrix[0] == NULL)
		{	
			SplitQuadtree(quad); 
 
			while (i < objects) /* relocate any applicable objects */
			{
				index = GetQuadtreeIndex(&quad->list[i]->rect,quad); 
				if (index != -1)
				{
					InsertCollider(quad->list[i], quad->submatrix[index]); /* move the collider into the lower levels */
					quad->list[i] = NULL; /* remove previous entity that has moved down a level */
				}
				i++;
			}
		}
	}
}

/*
 * Return all objects that could potentially collide with the given object
 */
void RetrieveCollisions(AABB* rect, Quadtree* quad, Entity* (*ents)[], int cur) /* a pointer to an array of Entity pointers */
{
	int i;

	int index = GetQuadtreeIndex(rect, quad);
	if (index != -1 && quad->submatrix[0] != NULL) RetrieveCollisions(rect, quad->submatrix[index], ents, cur);

	while((*ents)[i] = quad->list[i])
	{
		cur++; /* this is only safe because the quadtree is recreated each frame */
	}
}