#include "Player.h"
#include "Engine/Engine.h"
#include "Game/Game.h"
#include "Level/GameLevel.h"

Player::Player(const Vector2& position, GameLevel* level)
	: DrawableActor("P"), refLevel(level)
{
	// 위치 설정
	this->position = position;

	// 색상 설정
	color = Color::White;
}

void Player::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// ESC 종료
	if (Engine::Get().GetKeyDown(VK_ESCAPE))
	{
		// 메뉴 토글
		Game::Get().ToggleMenu();
	}

	// 상하좌우 이동
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
