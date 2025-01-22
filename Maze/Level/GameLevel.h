#pragma once

#include "Level/Level.h"
#include "Actor/DrawableActor.h"
#include <vector>

#pragma region Ŭ���� ���漱��
class Enemy;
class Goal;
class Player;
#pragma endregion

class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel(int stageNum, const std::string& fileName, int startX, int startY, int width = 350, int height = 350);

	// ������Ʈ �Լ�
	virtual void Update(float deltaTime) override;

	// ������ �׸��� �Լ�
	virtual void Draw() override;

	// �÷��̾ �̵��� �������� Ȯ���ϴ� �Լ�
	bool CanPlayerMove(const Vector2& position);

	// ���ʹ̰� �̵��� �������� Ȯ���ϴ� �Լ�
	bool CanEnemyMove(const Vector2& position);

	// �ܼ�â �̵� �Լ�
	void MoveConsole(int dx,int dy);

	// �ܼ�â ��ġ �� ũ�� ���� �Լ�
	//void SetConsoleWindow(int x,int y,int width,int height);

	// ���� Ŭ���� Ȯ�� �Լ�
	bool CheckGameClear();

	int mapWidth;  // �� �ʺ�
	int mapHeight; // �� ����
	int consoleX;  // �ܼ� X ��ǥ
	int consoleY;  // �ܼ� Y ��ǥ
	int consoleWidth;  // �ܼ� �ʺ�
	int consoleHeight; // �ܼ� ����

private:
	HWND console;  // �ܼ� �ڵ�

	std::vector<std::vector<Actor*>> mapData;
	std::vector<Enemy*> enemies;
	std::vector<Goal*> goals;
	Player* player = nullptr;

	// ���� Ŭ���� ����
	bool isGameClear = false;

	int stageNum;
	float playTime = 0;
};

