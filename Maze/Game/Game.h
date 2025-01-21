#pragma once

#include <Engine/Engine.h>

class Game : public Engine
{
public:
	Game();
	~Game();

	void ToggleMenu();

	void LoadStage(int stageNumber);

	void LoadClear(int stageNumber);

	static Game& Get() { return *instance; }

	void SetConsoleWindow(int x,int y,int width,int height);

private:
	bool showMenu = false;

	Level* menuLevel = nullptr;
	Level* backLevel = nullptr;

private:
	static Game* instance;
};