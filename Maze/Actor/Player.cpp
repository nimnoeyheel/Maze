#include "Player.h"
#include "Engine/Engine.h"
#include "Game/Game.h"
#include "Level/GameLevel.h"

Player::Player(const Vector2& position, GameLevel* level)
	: DrawableActor("P"), refLevel(level)
{
	// ��ġ ����
	this->position = position;

	// ���� ����
	color = Color::White;
}

void Player::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// ESC ����
	if (Engine::Get().GetKeyDown(VK_ESCAPE))
	{
		//Engine::Get().QuitGame();

		// �޴� ���
		Game::Get().ToggleMenu();
	}

	// �����¿� �̵�
	if (Engine::Get().GetKey(VK_LEFT))
	{
		refLevel->MoveConsole(-5, 0);
	}
	if (Engine::Get().GetKey(VK_RIGHT))
	{
		refLevel->MoveConsole(5,0);
	}
	if (Engine::Get().GetKey(VK_UP))
	{
		refLevel->MoveConsole(0, -5);
	}
	if (Engine::Get().GetKey(VK_DOWN))
	{
		refLevel->MoveConsole(0, 5);
	}
}
