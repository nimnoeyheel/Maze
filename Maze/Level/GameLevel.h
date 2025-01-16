#pragma once

#include "Level/Level.h"
#include "Actor/DrawableActor.h"

#pragma region 클래스 전방선언
class DrawableActor;
class Player;
#pragma endregion

class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel(const char* fileName, int startX, int startY, int width = 350, int height = 350);

	// 업데이트 함수
	virtual void Update(float deltaTime) override;

	// 레벨의 그리기 함수
	virtual void Draw() override;

	// 플레이어가 이동이 가능한지 확인하는 함수
	bool CanPlayerMove(const Vector2& position);

	// 콘솔창 이동 함수
	void MoveConsole(int dx, int dy);

private:

	HWND console;  // 콘솔 핸들
	int consoleX;  // 콘솔 X 좌표
	int consoleY;  // 콘솔 Y 좌표
	int consoleWidth;  // 콘솔 너비
	int consoleHeight; // 콘솔 높이

	const char* mapFileName; // 맵 파일 이름

	// 벽, 땅 액터 배열
	List<DrawableActor*> map;

	// 플레이어 액터
	Player* player = nullptr;

	// 게임 클리어 변수
	bool isGameClear = false;

};

