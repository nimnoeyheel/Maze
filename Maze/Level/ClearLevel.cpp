#include "ClearLevel.h"
#include "Engine/Engine.h"
#include "Level/GameLevel.h"
#include "Game/Game.h"
#include <string>


ClearLevel::ClearLevel(int _stageNum, int _score)
	: stageNum(_stageNum), score(_score)
{
	menuItems.push_back(new MenuItem("Next Stage",this,&ClearLevel::LoadNextStage,stageNum));
	menuItems.push_back(new MenuItem("Quit Game",+[](int) { Game::Get().QuitGame(); },0));

	length = menuItems.size();
}

ClearLevel::~ClearLevel()
{
	for(auto* item : menuItems)
	{
		delete item;if(Game::Get().GetKeyDown(VK_RETURN))
		{
			menuItems[currentIndex]->Execute();
		}
	}
}

void ClearLevel::Update(float deltaTime)
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
		//menuItems[currentIndex]->onSelected();
		menuItems[currentIndex]->Execute();
	}
}

void ClearLevel::Draw()
{
	Super::Draw();

	// 텍스트 출력 시작 위치
	int startX = 0,startY = 0;

	// 클리어 스테이지 로그
	std::string clearLog = "Clear Stage " + std::to_string(stageNum);
	Engine::Get().Draw(Vector2(startX,startY),clearLog.c_str(),unselectedColor);

	// 스코어 로그
	std::string scoreLog = "Score : " + std::to_string(score);
	Engine::Get().Draw(Vector2(startX,startY + 2),scoreLog.c_str(),unselectedColor);

	// 메뉴 로그
	for(int i = 0; i < length; ++i)
	{
		Color textColor = (i == currentIndex) ? selectedColor : unselectedColor;
		Engine::Get().Draw(Vector2(startX,startY + i + 4),menuItems[i]->menuText,textColor);
	}
}

void ClearLevel::LoadNextStage(int stageNum)
{
	Game::Get().LoadStage(stageNum+1);
}

void ClearLevel::QuitGame(int)
{
	Game::Get().QuitGame();
}
