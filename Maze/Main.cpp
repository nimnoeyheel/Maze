#include <iostream>
#include "Engine/Engine.h"
#include "Game/Game.h"
#include "Level/StartLevel.h"

int main()
{
	// 메모리 누수 확인
	CheckMemoryLeak();

	Game game;
	game.LoadLevel(new StartLevel());
	game.Run();

	std::cin.get();
}