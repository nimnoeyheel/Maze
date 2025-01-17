#pragma once

#include "Actor/DrawableActor.h"

// 플레이어의 목표 지점 액터
class Goal : public DrawableActor
{
	RTTI_DECLARATIONS(Goal, DrawableActor)

public:
	Goal(const Vector2& position);
};