#include "Box.h"

Box::Box(const Vector2& position)
	: DrawableActor("��")
{
	// ��ġ ����
	this->position = position;

	// ���� ����
	color = Color::Green;
}