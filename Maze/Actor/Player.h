#pragma once

#include "Actor/DrawableActor.h"
#include "Level/GameLevel.h"

class Projectile;

class Player : public DrawableActor
{
	RTTI_DECLARATIONS(Player, DrawableActor)

public:
	Player(const Vector2& position, GameLevel* level);

	virtual void Update(float deltaTime) override;

	int GetHP() { return hp; }
	int GetSeeds() { return seeds; }
	int GetTakeEnemy() { return takeEnemies; }

	void TakeDamage() { --hp; }
	void TakeSeed() { ++seeds; }
	void TakeEnemise() { ++takeEnemies; }

	std::vector<Projectile*>& GetProjectiles() { return projectiles; }

private:
	// 게임 레벨을 참조하는 변수
	GameLevel* refLevel = nullptr;
	
	int hp = 3;
	int seeds = 0;
	int takeEnemies = 0;

	// 마지막 방향 저장
	Vector2 lastDirection = Vector2(0,0);

	float currentTime = 0.0f;
	float deltaTime = 0.0f;

	// 발사체 저장
	std::vector<Projectile*> projectiles;
	void Attack();
	void UpdateProjectiles();
};