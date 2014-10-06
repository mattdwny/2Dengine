#include "AABB.h"

int boxOnBox(AABB first, AABB second)
{
    // Collision tests
    if( first.maxX < second.minX || first.minX > second.maxX ) return false;
    if( first.maxY < second.minY || first.minY > second.maxY ) return false;

    return true;
}