#pragma once

#include "Level/Level.h"
#include "Actor/DrawableActor.h"
#include <vector>

#pragma region Ŭ���� ���漱��
class DrawableActor;
class Player;
#pragma endregion

class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel(const std::string& fileName, int startX, int startY, int width = 350, int height = 350);

	// ������Ʈ �Լ�
	virtual void Update(float deltaTime) override;

	// ������ �׸��� �Լ�
	virtual void Draw() override;

	// �÷��̾ �̵��� �������� Ȯ���ϴ� �Լ�
	bool CanPlayerMove(const Vector2& position);

	// �ܼ�â �̵� �Լ�
	void MoveConsole(int dx, int dy);


private:

	int mapWidth;  // �� �ʺ�
	int mapHeight; // �� ����

	HWND console;  // �ܼ� �ڵ�
	int consoleX;  // �ܼ� X ��ǥ
	int consoleY;  // �ܼ� Y ��ǥ
	int consoleWidth;  // �ܼ� �ʺ�
	int consoleHeight; // �ܼ� ����

	// ���� �����͸� �����ϴ� 2D �����̳�
	std::vector<std::vector<Actor*>> mapData;

	// �÷��̾� ����
	Player* player = nullptr;

	// ���� Ŭ���� ����
	bool isGameClear = false;

};

