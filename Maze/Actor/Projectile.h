#pragma once

#include "Actor/DrawableActor.h"

class Projectile: public DrawableActor
{
	RTTI_DECLARATIONS(Projectile,DrawableActor)

public:
	Projectile(const Vector2& position);

	bool UpdateProjectile(float deltaTime, float currentTime);

	float GetStartTime() const { return startTime; }
	void SetStartTime(float time) { startTime = time; }

private:
	float lifetime = 0.1f; // 기본 수명
	float startTime = -1; // 발사 시작 시간
};

