#pragma once

#include "Actor/DrawableActor.h"

class Projectile: public DrawableActor
{
	RTTI_DECLARATIONS(Projectile,DrawableActor)

public:
	Projectile(const Vector2& position);

	bool UpdateProjectile(float deltaTime, float currentTime);

	void SetStartTime(float time) { startTime = time; }
	float GetStartTime() const { return startTime; }

private:
	float lifetime = 0.2f; // 기본 수명
	float startTime = 0.0f; // 발사 시작 시간
};

