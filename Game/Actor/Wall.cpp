#include "Wall.h"

Wall::Wall(const Vector2& position)
	: DrawableActor("|")
{
	// ��ġ ����
	this->position = position;

	// ���� ����
	color = Color::Green;
}