#ifndef AABB_H
#define AABB_H

typedef struct AABB_S
{
	float min[2];
	float max[2];
} AABB;

int BoxOnBox(AABB* first, AABB* second);
void AABBtoCircle(AABB* rect, float* x, float* y, float* r);
int CircleOnCircle(float* a, float* b, float ar, float br, float* outX, float* outY);

#endif