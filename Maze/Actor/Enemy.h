#pragma once

#include "Actor/DrawableActor.h"
#include "Level/GameLevel.h"

class Enemy: public DrawableActor
{
	enum class DirectionGroup
	{
		None = -1,
		UpAndDown,
		LeftAndRight
	};

	enum class Direction
	{
		None = -1,
		Up,
		Down,
		Left,
		Right
	};

	RTTI_DECLARATIONS(Enemy,DrawableActor)

public:
	Enemy(const Vector2& position, GameLevel* level);

	virtual void Update(float deltaTime) override;

private:
	// 게임 레벨을 참조하는 변수
	GameLevel* refLevel = nullptr;

	// 이동 방향
	DirectionGroup directionGroup = DirectionGroup::None;
	Direction direction = Direction::None;

	float speed = 5;

	void ReverseDirection();
};

