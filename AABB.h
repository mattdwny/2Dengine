#ifndef AABB_H
#define AABB_H

typedef struct AABB_S
{
	int minX;
	int maxX;
	int minY;
	int maxY;
} AABB;

int boxOnBox(AABB first, AABB second);

#endif