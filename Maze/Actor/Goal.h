#pragma once

#include "Actor/DrawableActor.h"

// �÷��̾��� ��ǥ ���� ����
class Goal : public DrawableActor
{
	RTTI_DECLARATIONS(Goal, DrawableActor)

public:
	Goal(const Vector2& position);
};