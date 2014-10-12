#include "AABB.h"

int boxOnBox(AABB first, AABB second)
{
    // Collision tests
    if( first.max[0] < second.min[0] || first.min[0] > second.max[0] ) return false;
    if( first.max[1] < second.min[1] || first.min[1] > second.max[1] ) return false;

    return true;
}