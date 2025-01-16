#include "StartLevel.h"
#include "Level/GameLevel.h"
#include "Game/Game.h"
#include "Engine/Engine.h"

StartLevel::StartLevel()
{
	// �ܼ� â ũ��� ��ġ ����
	SetConsoleWindowToCenter(800, 500);

	menuItems.PushBack(new MenuItem("Start Game", []() { Game::Get().LoadStage(1); }));
	menuItems.PushBack(new MenuItem("Quit Game", []() { Game::Get().QuitGame(); }));
	length = menuItems.Size();
}

StartLevel::~StartLevel()
{
	for (auto* item : menuItems)
	{
		delete item;
	}
}

void StartLevel::Update(float deltaTime)
{
	/*if (Game::Get().GetKeyDown(VK_ESCAPE))
	{
		Game::Get().ToggleMenu();
	}*/

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

	Engine::Get().SetCursorPosition(0, 0);

	SetColor(unselectedColor);
	Log("Escape The Maze\n\n");

	for (int ix = 0; ix < length; ++ix)
	{
		SetColor(ix == currentIndex ? selectedColor : unselectedColor);
		Log("%s\n", menuItems[ix]->menuText);
	}
}

void StartLevel::SetConsoleWindowToCenter(int pixelWidth, int pixelHeight)
{
	// ���ο� �ܼ� â ����
	AllocConsole();

	// �ܼ� �ڵ� ��������
	HWND consoleWindow = GetConsoleWindow();
	if (consoleWindow == NULL) return;

	// ȭ�� ũ�� ��������
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// �ܼ� â ũ�� ����
	COORD bufferSize = { (SHORT)pixelWidth, (SHORT)pixelHeight };
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);

	SMALL_RECT rect = { 0, 0, (SHORT)pixelWidth - 1, (SHORT)pixelHeight - 1 };
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &rect);

	// �߾� ��ġ ���
	int x = (screenWidth - pixelWidth) / 2;
	int y = (screenHeight - pixelHeight) / 2;

	// â ��ġ �̵�
	MoveWindow(consoleWindow, x, y, pixelWidth, pixelHeight, TRUE);
}
