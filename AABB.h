#ifndef AABB_H
#define AABB_H

typedef struct AABB_S
{
	float min[2];
	float max[2];
} AABB;

int BoxOnBox(AABB* first, AABB* second);

#endif