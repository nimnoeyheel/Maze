#include "Goal.h"

Goal::Goal(const Vector2& position)
	:DrawableActor("Goal")
{
	// ��ġ ����
	this->position = position;

	// ���� ����
	color = Color::Red;
}