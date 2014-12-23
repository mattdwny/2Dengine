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

/*void AABBtoCircle(AABB* rect, float* x, float* y, float* r)
{
	*x = (rect->min[0] + rect->max[0])/2;
	*y = (rect->min[1] + rect->max[1])/2;
	*r = *x - rect->min[0];
}*/

int CircleOnCircle(float* a, float* b, float ar, float br, float* outX, float* outY)
{
	float x,y,r,magSqr;

	x = b[0] - a[0];
	y = b[1] - a[1];
	
	r = ar + br;

	magSqr = x*x + y*y;

	if(magSqr <= r*r)
	{
		magSqr = sqrt(magSqr) - ar; //get the distance the hitbox (b) has embedded into the hurtbox (a)
		x /= magSqr;
		y /= magSqr;
		*outX = x;
		*outY = y;
		return 1;
	}

	return 0;
}
