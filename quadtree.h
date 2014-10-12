#ifndef QUADTREE_H
#define QUADTREE_H

#include <stdlib.h>
#include <math.h>
#include "AABB.h"
#include "entity.h"

/**
 * An spacial partitioning entity capable of subdiving space into quarters: http://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374
 */
typedef struct Quadtree_S /* note: 1 + 1 + 1 + 21 + 1*4 + 4*1 = 32 */
{
	int depth; /** number of quadtrees dereferences to get to target quadtree from root */
	int objects; /** the preferred number of objects that should reside within the quadtree */
	AABB  bounds; /** the encapsulating boundaries of the quadtree */
	struct Quadtree_S* next; /** pointer to the next free quadtree */
	Entity* list[21];  /** a list of entities that reside entirely inside the quadtrees boundaries */
	struct Quadtree_S* submatrix[4]; /** the array of pointers to subtrees */
} Quadtree;



#endif