#ifndef VECTOR2_H
#define VECTOR2_H

#include <math.h>

typedef float vec2d[2];

int VectorCompare (vec2d v1, vec2d v2);
int VectorApprox (vec2d v1, vec2d v2);
float VectorMagnitude (vec2d v);
float DotProduct (vec2d v1, vec2d v2);

void VectorNormalize (vec2d v); //XXX: no out
void VectorInverse (vec2d v); //XXX: no out

void CrossProduct (vec2d v, vec2d cross);
void VectorSubtract (vec2d vec1, vec2d vec2, vec2d out);
void VectorAdd (vec2d vec1, vec2d vec2, vec2d out);
void VectorScale (vec2d v, float scale, vec2d out);
void VectorCopy (vec2d in, vec2d out);

#endif