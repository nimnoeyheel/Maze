#pragma once

#include "Actor/DrawableActor.h"
#include "Level/GameLevel.h"

class Enemy: public DrawableActor
{
	RTTI_DECLARATIONS(Enemy,DrawableActor)

public:
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

public:
	Enemy(const Vector2& position, GameLevel* level);

	virtual void Update(float deltaTime) override;

	// 이동 방향
	DirectionGroup directionGroup = DirectionGroup::None;
	Direction direction = Direction::None;

	Vector2 GetPosition() const { return position; }

	Enemy::Direction GetDirection() const { return direction; };
	void ReverseDirection();

private:
	// 게임 레벨을 참조하는 변수
	GameLevel* refLevel = nullptr;

	float speed = 0.1f;

};

