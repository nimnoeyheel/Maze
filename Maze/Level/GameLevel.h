#pragma once

#include "Level/Level.h"
#include "Actor/DrawableActor.h"

#pragma region Ŭ���� ���漱��
class DrawableActor;
class Player;
#pragma endregion

class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel(const char* fileName, int startX, int startY, int width = 350, int height = 350);

	// ������Ʈ �Լ�
	virtual void Update(float deltaTime) override;

	// ������ �׸��� �Լ�
	virtual void Draw() override;

	// �÷��̾ �̵��� �������� Ȯ���ϴ� �Լ�
	bool CanPlayerMove(const Vector2& position);

	// �ܼ�â �̵� �Լ�
	void MoveConsole(int dx, int dy);

private:

	HWND console;  // �ܼ� �ڵ�
	int consoleX;  // �ܼ� X ��ǥ
	int consoleY;  // �ܼ� Y ��ǥ
	int consoleWidth;  // �ܼ� �ʺ�
	int consoleHeight; // �ܼ� ����

	const char* mapFileName; // �� ���� �̸�

	// ��, �� ���� �迭
	List<DrawableActor*> map;

	// �÷��̾� ����
	Player* player = nullptr;

	// ���� Ŭ���� ����
	bool isGameClear = false;

};

