#pragma once

#include "Level/Level.h"
#include "Actor/DrawableActor.h"
#include <vector>

#pragma region 클래스 전방선언
class Enemy;
class Goal;
class Player;
#pragma endregion

class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel(int stageNum, const std::string& fileName, int startX, int startY, int width = 350, int height = 350);

	// 업데이트 함수
	virtual void Update(float deltaTime) override;

	// 레벨의 그리기 함수
	virtual void Draw() override;

	// 플레이어가 이동이 가능한지 확인하는 함수
	bool CanPlayerMove(const Vector2& position);

	// 에너미가 이동이 가능한지 확인하는 함수
	bool CanEnemyMove(const Vector2& position);

	// 에너미가 벽에 부딪혔는지 확인하는 함수
	bool EnemyOverlapWall(const Vector2& position);

	// 게임 클리어 확인 함수
	bool CheckGameClear();

	// 게임 오버 확인 함수
	bool CheckGameOver();

	// 콘솔창 이동 함수
	void MoveConsole(int dx,int dy);

	int mapWidth;  // 맵 너비
	int mapHeight; // 맵 높이
	int consoleX;  // 콘솔 X 좌표
	int consoleY;  // 콘솔 Y 좌표
	int consoleWidth;  // 콘솔 너비
	int consoleHeight; // 콘솔 높이

	std::vector<std::vector<Actor*>> mapData;
	std::vector<Enemy*> enemies;

private:
	HWND console;  // 콘솔 핸들

	std::vector<Goal*> goals;
	Player* player = nullptr;

	// 게임 클리어, 오버 변수
	bool isGameClear = false;
	bool isGameOver = false;

	int stageNum;
	float timeLimit = 60;
};

