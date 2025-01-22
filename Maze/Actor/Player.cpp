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

	Vector2 direction(0,0);

	// 상하좌우 이동
	if (Engine::Get().GetKey(VK_LEFT))
	{
		direction = Vector2(-1, 0);
	}
	if (Engine::Get().GetKey(VK_RIGHT))
	{
		direction = Vector2(1, 0);
	}
	if (Engine::Get().GetKey(VK_UP))
	{
		direction = Vector2(0,-1);
	}
	if (Engine::Get().GetKey(VK_DOWN))
	{
		direction = Vector2(0, 1);
	}

	// 이동 방향이 설정되었으면 이동 처리
	if(direction != Vector2(0,0))
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		float scaleX = static_cast<float>(screenWidth) / refLevel->mapWidth;
		float scaleY = static_cast<float>(screenHeight) / refLevel->mapHeight;
		float HalfX = 350 / 16 / 2;
		float HalfY = 350 / 16 / 2;
		float mapX = refLevel->consoleX / scaleX + HalfX;
		float mapY = refLevel->consoleY / scaleY + HalfY;

		Vector2 newPosition = Vector2(static_cast<int>(mapX),static_cast<int>(mapY)) + direction;
		
		if(refLevel->CanPlayerMove(newPosition))
		{
			SetPosition(newPosition);
			refLevel->MoveConsole(direction.x * 5,direction.y * 5);
		}
	}
}