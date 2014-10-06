#include "vector2.h"

int VectorCompare (vec2d v1, vec2d v2)
{
	if (v1[0] != v2[0] || v1[1] != v2[1]) return 0;
	return 1;
}

int VectorApprox (vec2d v1, vec2d v2)
{
	if ( abs(v1[0] - v2[0]) < 0.01f || abs(v1[1] - v2[1]) < 0.01f ) return 0;
	return 1;
}

void VectorNormalize (vec2d v)
{
	float length = VectorMagnitude(v);

	if (length)
	{
		v[0] /= length;
		v[1] /= length;
	}
}

float VectorMagnitude (vec2d v)
{
	return sqrt(v[0] * v[0] + v[1] * v[1]);
}

float DotProduct (vec2d v1, vec2d v2)
{
	return v1[0]*v2[0] + v1[1]*v2[1];
}

void VectorSubtract (vec2d vec1, vec2d vec2, vec2d out)
{
	out[0] = vec1[0] - vec2[0];
	out[1] = vec1[1] - vec2[1];
}

void VectorAdd (vec2d vec1, vec2d vec2, vec2d out)
{
	out[0] = vec1[0] + vec2[0];
	out[1] = vec1[1] + vec2[1];
}

void VectorScale (vec2d v, float scale, vec2d out)
{
	out[0] = v[0] * scale;
	out[1] = v[1] * scale;
}

void VectorCopy (vec2d in, vec2d out)
{
	out[0] = in[0];
	out[1] = in[1];
}

void CrossProduct (vec2d v, vec2d cross)
{
	cross[0] = -v[1]; //negative reciprocal, rotates 90 degrees CCW (counter-clockwise)
	cross[1] =  v[0];
}

void VectorInverse (vec2d v)
{
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}