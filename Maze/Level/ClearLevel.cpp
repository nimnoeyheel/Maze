#include "ClearLevel.h"
#include "Engine/Engine.h"
#include "Level/GameLevel.h"
#include "Game/Game.h"
#include <string>


ClearLevel::ClearLevel(int _stageNum, int _score, int _playTime)
	: stageNum(_stageNum), score(_score), playTime(_playTime)
{
	//menuItems.push_back(new MenuItem("Next Stage",this,&ClearLevel::LoadNextStage,stageNum));

	if(stageNum < 3)
	{
		menuItems.push_back(new MenuItem("Next Stage",[](int number) { Game::Get().LoadStage(++number); },stageNum));
		menuItems.push_back(new MenuItem("Quit Game",+[](int) { Game::Get().QuitGame(); },0));

	}
	else
	{
		menuItems.push_back(new MenuItem("Quit Game",+[](int) { Game::Get().QuitGame(); },0));
	}

	length = menuItems.size();
}

ClearLevel::~ClearLevel()
{
	for(auto* item : menuItems)
	{
		delete item;
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
	std::string scoreLog = "Total Score : " + std::to_string(score);
	Engine::Get().Draw(Vector2(startX,startY + 2),scoreLog.c_str(),Color::White);

	// 플레이 타임 로그
	int totalSeconds = playTime;
	int minutes = totalSeconds / 60;
	int seconds = totalSeconds % 60;
	std::string playTimeLog = "Play Time : ";

	if(minutes > 0)
	{
		playTimeLog += std::to_string(minutes) + "min " + std::to_string(seconds) + "sec";
	}
	else
	{
		playTimeLog += std::to_string(seconds) + "sec";
	}
	Engine::Get().Draw(Vector2(startX,startY + 3),playTimeLog.c_str(),Color::White);

	// 메뉴 로그
	for(int i = 0; i < length; ++i)
	{
		Color textColor = (i == currentIndex) ? selectedColor : unselectedColor;
		Engine::Get().Draw(Vector2(startX,startY + i + 5),menuItems[i]->menuText,textColor);
	}
}
