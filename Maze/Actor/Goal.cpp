#include "Goal.h"

Goal::Goal(const Vector2& position)
	:DrawableActor("Goal")
{
	// 위치 설정
	this->position = position;

	// 색상 지정
	color = Color::Red;
}