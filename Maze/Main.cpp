#include <iostream>
#include "Engine/Engine.h"
#include "Game/Game.h"
#include "Level/StartLevel.h"

int main()
{
	// �޸� ���� Ȯ��
	CheckMemoryLeak();

	Game game;
	game.LoadLevel(new StartLevel());
	game.Run();

	std::cin.get();
}