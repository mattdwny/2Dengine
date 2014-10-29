#include <stdlib.h>
#include <math.h>
#include "AABB.h"

int BoxOnBox(AABB* first, AABB* second)
{
    // Collision tests
    if( first->max[0] < second->min[0] || first->min[0] > second->max[0] ) return false;
    if( first->max[1] < second->min[1] || first->min[1] > second->max[1] ) return false;

    return true;
}

void AABBtoCircle(AABB* rect, float* x, float* y, float* r)
{
	*x = (rect->min[0] + rect->max[0])/2;
	*y = (rect->min[1] + rect->max[1])/2;
	*r = *x - rect->min[0];
}

int CircleOnCircle(float x1, float y1, float r1, float x2, float y2, float r2, float* outX, float* outY)
{
	float x,y,r,magSqr;

	x = x2 - x1;
	y = y2 - y1;
	
	r = r1 + r2;

	magSqr = x*x + y*y;

	if(magSqr > r*r)
	{
		*outX = x;
		*outY = y;
		return 1;
	}

	return 0;
}
