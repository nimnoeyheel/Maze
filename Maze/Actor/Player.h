#pragma once

#include "Actor/DrawableActor.h"
#include "Level/GameLevel.h"

class Player : public DrawableActor
{
	RTTI_DECLARATIONS(Player, DrawableActor)

public:
	Player(const Vector2& position, GameLevel* level);

	virtual void Update(float deltaTime) override;

	int GetHP() { return hp; }
	int GetSeeds() { return seeds; }

	void TakeDamage() { if(hp > 0) --hp; }
	void TakeSeed() { ++seeds; }

private:
	// 게임 레벨을 참조하는 변수
	GameLevel* refLevel = nullptr;
	
	int hp = 3;
	int seeds = 0;
};