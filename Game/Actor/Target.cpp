#include "Target.h"

Target::Target(const Vector2& position)
	: DrawableActor("��")
{
	// ��ġ ����
	this->position = position;

	// ���� ����
	color = Color::Red;
}