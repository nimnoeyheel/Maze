#include "Projectile.h"

Projectile::Projectile(const Vector2 & position)
	: DrawableActor("*")
{
	this->position = position;
	color = Color::White;
}

bool Projectile::UpdateProjectile(float deltaTime, float currentTime)
{
	// 발사 조건 : 렌더링 시작 시간이 현재 시간보다 작아야 렌더링 시작
	if(currentTime >= startTime)
	{
		lifetime -= deltaTime;
		return lifetime <= 0; // 수명 초과 시 제거
	}

	return false; // 아직 발사되지 않음
}