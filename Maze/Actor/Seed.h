#pragma once

#include "Actor/DrawableActor.h"

class Seed : public DrawableActor
{
	RTTI_DECLARATIONS(Seed, DrawableActor)

public:
	Seed(const Vector2& position);
};