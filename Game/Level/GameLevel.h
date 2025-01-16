#pragma once

#include "Level/Level.h"
#include "Math/Vector2.h"

// 소코반 게임 레벨

#pragma region 클래스 전방선언
class DrawableActor;
class Player;
class Box;
class Target;
#pragma endregion

class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();

	// 업데이트 함수
	virtual void Update(float deltaTime) override;

	// 레벨의 그리기 함수
	virtual void Draw() override;

	// 플레이어가 이동이 가능한지 확인하는 함수
	bool CanPlayerMove(const Vector2& position);

private:
	// 박스를 옮긴 뒤 게임을 클리어 했는지 확인하는 함수
	bool CheckGameClear();

	// 벽, 땅 액터 배열
	List<DrawableActor*> map;

	// 박스 액터
	List<Box*> boxes;

	// 타겟 액터
	List<Target*> targets;

	// 플레이어 액터
	Player* player = nullptr;

	// 게임 클리어 변수
	bool isGameClear = false;
};