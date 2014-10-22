#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "quadtree.h"
#include "game.h"
#include "graphics.h"

//extern ScreenData;

#define __maxCollisions 16 /*the maximum number of collisions allowed by the game engine*/
#define __maxDepth 3 /* the max number of quadtrees dereferences to get to target quadtree from root */
#define __targetSize 5 /* the preferred number of objects that should reside within the quadtree */
#define __maxQuadtrees (1 + 4 + 4*4 + 4*4*4) /* depth of 0, 1, 2, 3 respectively */

Quadtree __quadtrees[__maxQuadtrees]; /** the list of quadtrees */
int __nextFreeQuadtree; /** the first available quadtree in the quadtree list */

/**
 *  Return the next available quadtree
 */
Quadtree* GetQuadtree(int depth, float minWidth, float maxWidth, float minHeight, float maxHeight)
{
	if(__nextFreeQuadtree >= __maxQuadtrees) CRASH("Not enough Quadtrees allocated");

	Quadtree* quad = &__quadtrees[__nextFreeQuadtree];

	__nextFreeQuadtree++;

	quad->depth = depth;

	quad->bounds.min[0] = minWidth;
	quad->bounds.min[1] = minHeight;
	quad->bounds.max[0] = maxWidth;
	quad->bounds.max[1] = maxHeight;

	return quad;
}

/**
 * Clears all quadtrees, equivalent to a full wipe.
 */
void PrepareQuadtrees()
{
	memset(__quadtrees, 0, __maxQuadtrees * sizeof(Quadtree)); // this is safe because quadtrees are cleared between frames

	__nextFreeQuadtree = 0;

	//GetQuadtree(0 /*depth*/, 0, 0, ScreenData.xres, ScreenData.yres); //implicitly: __nextFreeQuadtree = 1;
}

/**
 * Splits the node into 4 subnodes
 */
void SplitQuadtree(Quadtree* quad)
{
	int nextDepth;
	float minWidth, minHeight, maxWidth, maxHeight, halfWidth, halfHeight; 

	nextDepth = quad->depth + 1;

	minWidth   = floor(quad->bounds.min[0]);
	minHeight  = floor(quad->bounds.min[1]);
	maxWidth   = ceil(quad->bounds.max[0] + 1);
	maxHeight  = ceil(quad->bounds.max[1] + 1);
	halfWidth  = floor((minWidth  + maxWidth ) / 2); // I can choose floor or ceil, the decision is irrelevant really
	halfHeight = floor((minHeight + maxHeight) / 2);
 
	quad->submatrix[0] = GetQuadtree(nextDepth, minWidth, halfWidth, minHeight, halfHeight); /* Create all four quadrant permutations */
	quad->submatrix[1] = GetQuadtree(nextDepth, halfWidth, maxWidth, minHeight, halfHeight);
	quad->submatrix[2] = GetQuadtree(nextDepth, minWidth, halfWidth, halfHeight, maxHeight);
	quad->submatrix[3] = GetQuadtree(nextDepth, halfWidth, maxWidth, halfHeight, maxHeight);
}

/**
 * Determine which node the object belongs to. -1 means object cannot completely fit within a child node and is part of the parent node
 */
int GetQuadtreeIndex(AABB* rect, Quadtree* quad)
{
	double halfX, halfY;
	int topQuadrant, bottomQuadrant;

	halfX = floor(( floor(quad->bounds.min[0]) + ceil(quad->bounds.max[0]) ) / 2); //Note: same notation as: halfWidth  = floor((minWidth  + maxWidth ) / 2);
	halfY = floor(( floor(quad->bounds.min[1]) + ceil(quad->bounds.max[1]) ) / 2);

	// Object can completely fit within the top quadrants
	topQuadrant = (rect->min[1] >= halfY); //sort of irrelevant, but since topQuad has less territory due to the floor function, I gave it rights to area in a tie, hence equals

	// Object can completely fit within the bottom quadrants
	bottomQuadrant = (rect->max[1] < halfY); 
 
	if (rect->max[0] < halfX) /* Object can completely fit within the left quadrants */
	{
		if      (topQuadrant)    return 0;
		else if (bottomQuadrant) return 2;
	}
	if (rect->min[0] >= halfX) /* Object can completely fit within the right quadrants */
	{
		if      (topQuadrant)    return 1;
		else if (bottomQuadrant) return 3;
	}
 
	return -1;
}

/**
 * Insert the object into the quadtree. If the node exceeds the capacity, it will split and add all objects to their corresponding nodes.
 */
 void InsertCollider(Entity* ent, Quadtree* quad)
 {
	int i;
	int index;
	int objects;
	Entity** spot;

	objects = quad->objects;

	if (quad->submatrix[0] != NULL) // either all quadtrees in submatrix are null or none are, hence why split creates 4 subtrees
	{
		index = GetQuadtreeIndex(&ent->rect, quad); // first try putting the object in the submatrix
		if (index != -1)
		{
			InsertCollider(ent, quad->submatrix[index]);
			return;
		}
	}
	
	if(objects >= 21) CRASH("FATAL ERROR: Too many objects in one quadtree");

	spot = &quad->list[0]; //pointer to the first entity pointer
	while(*spot != NULL) spot++; //check the next item in the list. Equivalent to (quad->list[0])-->(quad->list[1]), (quad->list[1])-->(quad->list[2])
	*spot = ent;
	quad->objects++;
	
	if (objects > __targetSize && quad->depth < __maxDepth)
	{
		if (quad->submatrix[0] == NULL) //only do the for loop when you split the quadtree, otherwise the add will properly delegate
		{
			SplitQuadtree(quad);
			
			for(i = 0; i < 21; i++) //these options opened up so consider reallocation for each
			{
				if(!quad->list[i]) continue;

				index = GetQuadtreeIndex(&quad->list[i]->rect,quad); 
				if (index != -1)
				{
					InsertCollider(quad->list[i], quad->submatrix[index]); //move the collider into the lower levels
					quad->list[i] = NULL; // remove previous entity that has moved down a level
					quad->objects--;
				}
			}
		}
	}
}

/**
 * Return all objects that could potentially collide with the given object
 */
void RetrieveCollisions(AABB* rect, Quadtree* quad, Entity* (*out)[__maxCollisions], /*a pointer to an array populated by (initially) NULL Entity pointers*/
						int cursor) /*starts at zero, moves forward as Entity pointers are added to the array of "ents"*/
{
	int i;
	int index;
	Entity* temp;

	for(i = 0; i < 21; i++) 
	{
		temp = quad->list[i];
		if(temp)
		{
			(*out)[cursor] = temp;
			cursor++;

			if(cursor >= __maxCollisions) CRASH("Too many collisions returned from Quadtree, there could be an issue.");
		}
	}

	index = GetQuadtreeIndex(rect, quad);
	if (index != -1 && quad->submatrix[0] != NULL) RetrieveCollisions(rect, quad->submatrix[index], out, cursor); //recursive call, which terminates at maxDepth or sooner
}

/**
 *  Overloaded retrieve collision function that hides the implementation of the previous, more complicated function.
 */
int RetrieveCollisions(Entity* ent, Entity* (*out)[__maxCollisions])
{
	int i, emptyIndex, filledIndex;

	RetrieveCollisions(&ent->rect, &__quadtrees[0], out, 0);

	for(i = 0; i < __maxCollisions; i++)
	{
		if(ent == (*out)[i])								(*out)[i] = NULL; //remove the entity's "self" from the list
		else if(!BoxOnBox(&ent->rect, &(*out)[i]->rect))	(*out)[i] = NULL; //do not return objects that don't collide
	}

	/*XXX: ERROR PRONE, should I keep this*/
	//rearrange everything for convenient usage
	for(emptyIndex = 0; emptyIndex < __maxCollisions; emptyIndex++)
	{
		while((*out)[emptyIndex] != NULL) //continue til you find something empty
		{
			emptyIndex++;
			if(emptyIndex >= __maxCollisions) goto end;
		} //at this point you found the first empty object remaining

		filledIndex = emptyIndex + 1; //make sure we don't swap "backwards"
		while((*out)[filledIndex] == NULL) //continue til you find something full
		{
			filledIndex++;
			if(filledIndex >= __maxCollisions) goto end;
		} //at this point you found the first filled object after the first empty object.

		(*out)[emptyIndex] = (*out)[filledIndex]; //Put the filled object where the empty object is
	}

	end:
	
	return emptyIndex; //if empty index were 0, there would be 0 collisions
}