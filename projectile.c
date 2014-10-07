#include "projectile.h"

void* ProjectileInit(void* data)
{
	if(!data) return NULL;

	Projectile* projectile = (Projectile*) data;

	//...

	return data;
}

void ProjectileDestroy(void* data)
{
	if(!data) return;

	Projectile* projectile = (Projectile*) data;
	//...
}

void ProjectileDraw(void* data)
{
	if(!data) return;

	Projectile* projectile = (Projectile*) data;

	//...
}

void ProjectileThink(void* data)
{
	if(!data) return;

	Projectile* projectile = (Projectile*) data;
	//...
}

void ProjectileUpdate(void* data)
{
	if(!data) return;

	Projectile* projectile = (Projectile*) data;

	//...
}