#include "FailedLevel.h"
#include "Engine/Engine.h"
#include "Level/GameLevel.h"
#include "Game/Game.h"
#include <string>

FailedLevel::FailedLevel(int _stageNum)
	: stageNum(_stageNum)
{
	menuItems.push_back(new MenuItems("Retry Game",[](int number) { Game::Get().LoadStage(number); },stageNum));
	menuItems.push_back(new MenuItems("Quit Game",+[](int) { Game::Get().QuitGame(); },0));

	length = menuItems.size();
}

FailedLevel::~FailedLevel()
{
	for(auto* item : menuItems)
	{
		delete item;
	}
}

void FailedLevel::Update(float deltaTime)
{
	if(Game::Get().GetKeyDown(VK_UP))
	{
		currentIndex = (currentIndex - 1 + length) % length;
	}
	if(Game::Get().GetKeyDown(VK_DOWN))
	{
		currentIndex = (currentIndex + 1) % length;
	}

	if(Game::Get().GetKeyDown(VK_RETURN))
	{
		menuItems[currentIndex]->Execute();
	}
}

void FailedLevel::Draw()
{
	Super::Draw();

	// 텍스트 출력 시작 위치
	int startX = 0,startY = 0;

	// 헤더 출력
	Engine::Get().Draw(Vector2(startX,startY+1),"Game Over~~~!!",unselectedColor);

	// 메뉴 로그
	for(int i = 0; i < length; ++i)
	{
		Color textColor = (i == currentIndex) ? selectedColor : unselectedColor;
		Engine::Get().Draw(Vector2(startX,startY + i + 5),menuItems[i]->menuText,textColor);
	}
}
