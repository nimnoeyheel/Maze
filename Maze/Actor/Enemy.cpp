#include "Enemy.h"
#include "Level/GameLevel.h"
#include <cstdlib>

Enemy::Enemy(const Vector2 & position, GameLevel* level)
	: DrawableActor("E") ,refLevel(level)
{
	this->position = position;
	color = Color::Purple;

	// 이동 그룹과 방향 랜덤으로 초기화
	int groupRandom = std::rand() % 2; // 0 또는 1
	directionGroup = (groupRandom == 0) ? DirectionGroup::UpAndDown : DirectionGroup::LeftAndRight;
	
	// 초기 방향 설정
	if(directionGroup == DirectionGroup::UpAndDown)
	{
		direction = (std::rand() % 2 == 0) ? Direction::Up : Direction::Down;
	}
	else
	{
		direction = (std::rand() % 2 == 0) ? Direction::Left : Direction::Right;
	}
}

void Enemy::Update(float deltaTime)
{
	Super::Update(deltaTime);

	Vector2 newPosition = position;

	// 현재 방향에 따라 이동
	switch(direction)
	{
	case Enemy::Direction::Up:
		newPosition.y -= speed * deltaTime;
		break;
	case Enemy::Direction::Down:
		newPosition.y += speed * deltaTime;
		break;
	case Enemy::Direction::Left:
		newPosition.x -= speed * deltaTime;
		break;
	case Enemy::Direction::Right:
		newPosition.x += speed * deltaTime;
		break;
	default:
		break;
	}

	// 벽 콜리전 체크
	if(refLevel && refLevel->CanPlayerMove(newPosition))
	{
		//position = newPosition;
		SetPosition(newPosition);
	}
	else
	{
		// 벽에 부딪히면 반대 방향으로 변경
		ReverseDirection();
	}

}

void Enemy::ReverseDirection()
{
	if(directionGroup == DirectionGroup::UpAndDown)
	{
		direction = (direction == Direction::Up) ? Direction::Down : Direction::Up;
	}
	else if(directionGroup == DirectionGroup::LeftAndRight)
	{
		direction = (direction == Direction::Left) ? Direction::Right : Direction::Left;
	}
}
