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
		SetConsoleWindow(100,100,350,350);
		newLevel = new GameLevel("../Assets/Maps/Stage1.txt", 100, 100, 350, 350);
		break;
	case 2:
		SetConsoleWindow(200,200,350,350);
		newLevel = new GameLevel("../Assets/Maps/Stage2.txt", 200, 200, 350, 350);
		break;
	case 3:
		SetConsoleWindow(0,630,350,350);
		newLevel = new GameLevel("../Assets/Maps/Stage3.txt", 0, 630, 350, 350);
		break;
	default:
		return;
	}

	Engine::Get().LoadLevel(newLevel); // 새로운 레벨 로드

}

void Game::SetConsoleWindow(int x,int y,int width,int height)
{
	HWND consoleWindow = GetConsoleWindow();
	if(consoleWindow == NULL) return;

	// 콘솔 폰트 크기 재설정으로 시작
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 8;
	cfi.dwFontSize.Y = 16;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName,L"Terminal");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE),FALSE,&cfi);

	// 버퍼 크기 계산
	int bufferWidth = width / cfi.dwFontSize.X;
	int bufferHeight = height / cfi.dwFontSize.Y;

	// screenSize 업데이트
	Engine::Get().ScreenSize() = Vector2(bufferWidth,bufferHeight);

	// 버퍼 크기 설정
	COORD bufferSize = {(SHORT)bufferWidth,(SHORT)bufferHeight};
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),bufferSize);

	// 창 크기 설정 - 버퍼와 동일하게
	SMALL_RECT windowRect = {0,0,(SHORT)bufferWidth - 1,(SHORT)bufferHeight - 1};
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE),TRUE,&windowRect);

	// 창 크기 및 위치 설정 - 실제 픽셀 크기로
	Sleep(50);  // 잠시 대기하여 이전 설정이 적용되도록 함

	// 실제 창 크기를 설정할 때는 윈도우 테두리 등을 고려
	RECT rect;
	GetWindowRect(consoleWindow,&rect);
	int borderWidth = GetSystemMetrics(SM_CXFRAME) * 2;
	int borderHeight = GetSystemMetrics(SM_CYFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
	int actualWidth = width + borderWidth;
	int actualHeight = height + borderHeight;

	MoveWindow(consoleWindow,x,y,actualWidth,actualHeight,TRUE);
	Sleep(50);  // 설정이 안정화될 시간을 줌
}