#ifndef AABB_H
#define AABB_H

typedef struct AABB_S
{
	float min[2];
	float max[2];
} AABB;

int BoxOnBox(AABB* first, AABB* second);
void AABBtoCircle(AABB* rect, float* x, float* y, float* r);
int CircleOnCircle(float x1, float y1, float r1, float x2, float y2, float r2, float* outX, float* outY);

#endif