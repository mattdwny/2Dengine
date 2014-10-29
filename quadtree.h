#ifndef QUADTREE_H
#define QUADTREE_H

#include <stdlib.h>
#include <math.h>
#include "AABB.h"

#define __listMax 12

typedef struct Info_s
{
	AABB* rect;
	void* ent;
} Info;

/**
 * An spacial partitioning entity capable of subdiving space into quarters: http://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374
 */
typedef struct Quadtree_S // note: 1*4 + 2*12 + 2*1 + 1 + 1 = 32
{
	struct Quadtree_S* submatrix[4]; /** the array of pointers to subtrees */
	Info list[__listMax];  /** a list of entities and AABBs that reside entirely inside the quadtrees boundaries */
	AABB  bounds; /** the encapsulating boundaries of the quadtree */
	int depth; /** number of quadtrees dereferences to get to target quadtree from root */
	int objects; /** the preferred number of objects that should reside within the quadtree */
} Quadtree;

void PrepareQuadtrees();
void InsertCollider(void* ent, AABB* rect);
void RetrieveCollisions(AABB* rect, void* (*out)[]);

#endif