#include "Seed.h"

Seed::Seed(const Vector2& position)
	: DrawableActor("S")
{
	this->position = position;
	color = Color::Yellow;
}