#include "StartLevel.h"
#include "Engine/Engine.h"
#include "Level/GameLevel.h"
#include "Game/Game.h"

StartLevel::StartLevel()
{
	menuItems.push_back(new MenuItem("Start Game", []() { Game::Get().LoadStage(1); }));
	menuItems.push_back(new MenuItem("Quit Game",[]() { Game::Get().QuitGame(); }));
	length = menuItems.size();
}

StartLevel::~StartLevel()
{
	while (!menuItems.empty())
	{
		delete menuItems[0];
		menuItems.erase(menuItems.begin());
	}

	//for (auto* item : menuItems)
	//{
	//	delete item;
	//}
}

void StartLevel::Update(float deltaTime)
{
	if (Game::Get().GetKeyDown(VK_UP))
	{
		currentIndex = (currentIndex - 1 + length) % length;
	}
	if (Game::Get().GetKeyDown(VK_DOWN))
	{
		currentIndex = (currentIndex + 1) % length;
	}

	if (Game::Get().GetKeyDown(VK_RETURN))
	{
		menuItems[currentIndex]->onSelected();
	}
}

void StartLevel::Draw()
{
	Super::Draw();

	// 텍스트 출력 시작 위치
	int startX = 0,startY = 0;

	// "Escape The Maze" 헤더 출력
	Engine::Get().Draw(Vector2(startX,startY),"Escape The Window",unselectedColor);

	// 메뉴 항목 출력
	for(int ix = 0; ix < length; ++ix) {
		Color textColor = (ix == currentIndex) ? selectedColor : unselectedColor;
		Engine::Get().Draw(Vector2(startX,startY + ix + 2),menuItems[ix]->menuText,textColor);
	}
}
