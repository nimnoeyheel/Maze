#include "Projectile.h"

Projectile::Projectile(const Vector2 & position)
	: DrawableActor("*")
{
	this->position = position;
	color = Color::White;
}

bool Projectile::UpdateProjectile(float deltaTime, float currentTime)
{
	if(currentTime >= startTime) // 발사 조건
	{
		lifetime -= deltaTime;
		return lifetime <= 0;
	}
	return false; // 아직 발사되지 않음
}