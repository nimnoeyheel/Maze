#include "Game.h"
#include "Engine/Engine.h"
#include "Level/StartLevel.h"
#include "Level/GameLevel.h"

Game* Game::instance = nullptr;

Game::Game()
	: Engine()
{
	instance = this;

	menuLevel = new StartLevel();
}

Game::~Game()
{
	if (showMenu)
	{
		delete backLevel;
		backLevel = nullptr;
		mainLevel = nullptr;
	}
	else
	{
		delete mainLevel;
		mainLevel = nullptr;
	}

	delete menuLevel;
	menuLevel = nullptr;
}

void Game::ToggleMenu()
{
	system("cls");

	showMenu = !showMenu;
	if (showMenu)
	{
		backLevel = mainLevel;
		mainLevel = menuLevel;
	}
	else
	{
		mainLevel = backLevel;
	}
}

void Game::LoadStage(int stageNumber)
{
	Level* newLevel = nullptr;

	switch (stageNumber) {
	case 1:
		newLevel = new GameLevel("../Assets/Maps/Stage1.txt", 100, 100, 350, 350);
		break;
	case 2:
		newLevel = new GameLevel("../Assets/Maps/Stage2.txt", 200, 200, 350, 350);
		break;
	case 3:
		newLevel = new GameLevel("../Assets/Maps/Stage3.txt", 300, 300, 350, 350);
		break;
	default:
		return;
	}

	Engine::Get().LoadLevel(newLevel); // 새로운 레벨 로드

}
