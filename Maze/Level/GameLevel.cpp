#include "GameLevel.h"
#include "Engine/Engine.h"
#include "Actor/Player.h"
#include "Engine/Timer.h"
#include "Actor/Wall.h"
#include "Actor/Ground.h"
#include "Actor/Goal.h"
#include <string>
#include <fstream>
#include <algorithm>

GameLevel::GameLevel(const std::string& fileName, int startX, int startY, int width, int height)
	: consoleX(startX), consoleY(startY), consoleWidth(width), consoleHeight(height)
{
	SetConsoleWindow(consoleX,consoleY, consoleWidth,consoleHeight);

	// 커서 감추기
	Engine::Get().SetCursorType(CursorType::NoCursor);

	// 맵 파일 읽기
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		std::cerr << "Failed to open map file: " << fileName << std::endl;
		__debugbreak();
	}

	int yPosition = 0; // y 좌표 초기화
	std::string line;

	// 파일 줄 단위로 읽기
	while (std::getline(file, line))
	{
		std::vector<Actor*> row; // 각 줄의 액터 저장
		for (int xPosition = 0; xPosition < static_cast<int>(line.size()); ++xPosition)
		{
			char mapChar = line[xPosition];

			// 문자에 따라 액터 생성
			if (mapChar == '1') {
				row.push_back(new Wall(Vector2(xPosition, yPosition)));
			}
			else if (mapChar == '.') {
				row.push_back(new Ground(Vector2(xPosition, yPosition)));
			}
			else if (mapChar == 'E') {
				row.push_back(new Goal(Vector2(xPosition, yPosition)));
			}
			else if (mapChar == 'P') {
				row.push_back(new Player(Vector2(xPosition, yPosition), this));
				player = dynamic_cast<Player*>(row.back());

				//consoleX = std::max(0, xPosition - consoleWidth / 2);
				//consoleY = std::max(0, yPosition - consoleHeight / 2);
			}
			else {
				row.push_back(nullptr); // 빈 공간
			}
		}
		mapData.push_back(row);
		++yPosition; // 다음 줄로 이동
	}

	// 맵 크기 설정
	mapHeight = static_cast<int>(mapData.size());
	mapWidth = mapData.empty() ? 0 : static_cast<int>(mapData[0].size());

    file.close();
}

void GameLevel::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// 플레이어 업데이트
	if (player)
	{
		player->Update(deltaTime);
	}

	// 게임이 클리어 됐으면 게임 종료 처리
	if (isGameClear)
	{
		// 타이머 클래스의 객체로 코드 정리
		static Timer timer(0.1);
		timer.Update(deltaTime);
		if (!timer.IsTimeOut()) return;

		// 커서 이동
		//Engine::Get().SetCursorPosition(0, Engine::Get().ScreenSize().y);

		// 메세지 출력
		Log("Game Clear!\n");

		// 쓰레드 정지
		Sleep(2000);

		// 게임 종료 처리
		Engine::Get().QuitGame();
	}

}

void GameLevel::Draw()
{
	// 맵 크기와 스크린 비율 계산
	float scaleX = static_cast<float>(screenWidth) / mapWidth;
	float scaleY = static_cast<float>(screenHeight) / mapHeight;

	// 화면에 보일 부분만 백 버퍼에 렌더링
	for (int y = 0; y < consoleHeight; ++y) {
		int mapY = consoleY / (scaleY) + y; // 맵의 Y 좌표 계산
		//int mapY = static_cast<int> (consoleY + y / scaleY); // 맵의 Y 좌표 계산
		if (mapY < 0 || mapY >= mapHeight) continue;  // mapData 범위 초과 시 무시

		for (int x = 0; x < consoleWidth; ++x) {
			int mapX = consoleX / (scaleX) + x ; // 맵의 X 좌표 계산
			//int mapX = static_cast<int> (consoleX + x / scaleX); // 맵의 X 좌표 계산
			if (mapX < 0 || mapX >= mapWidth) continue;  // mapData 범위 초과 시 무시

			// 맵 데이터에서 Actor 가져오기
			DrawableActor* actor = dynamic_cast<DrawableActor*>(mapData[mapY][mapX]);
			if(actor) {
				// Actor의 심볼과 색상을 백 버퍼에 쓰기
				Engine::Get().Draw(Vector2(x,y),actor->GetSymbol(),actor->GetColor());
			}
		}
	}

	// 플레이어 위치 중앙에 고정
	if (player)
	{
		int playerX = consoleWidth / 2;
		int playerY = consoleHeight / 2;
		player->SetPosition(Vector2(playerX, playerY));
		player->Draw();
	}

}

bool GameLevel::CanPlayerMove(const Vector2& position)
{
	// 게임이 클리어된 경우 바로 종료
	if (isGameClear) return false;


}

void GameLevel::MoveConsole(int dx, int dy)
{
	consoleX += dx;
	consoleY += dy;

	if (consoleX < 0) consoleX = 0;
	if (consoleY < 0) consoleY = 0;

	if(consoleX + consoleWidth > screenWidth) consoleX = screenWidth - consoleWidth;
	if(consoleY + consoleHeight > screenHeight) consoleY = screenHeight - consoleHeight;

	// 콘솔 창 이동
	HWND consoleWindow = GetConsoleWindow();
	if(consoleWindow == NULL) return;
	MoveWindow(consoleWindow,consoleX,consoleY,consoleWidth,consoleHeight,TRUE);
}

void GameLevel::SetConsoleWindow(int x,int y,int width,int height)
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
	
	//HWND consoleWindow = GetConsoleWindow();
	//if(consoleWindow == NULL) return;

	//// screenSize 업데이트
	//Vector2 newScreenSize(width / 8,height / 16);
	//Engine::Get().ScreenSize() = newScreenSize;

	//// 버퍼 크기를 새로운 창 크기와 일치시킴
	//COORD bufferSize = {(SHORT)newScreenSize.x,(SHORT)newScreenSize.y};
	//SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),bufferSize);

	//// 창 크기도 동일하게 설정
	//SMALL_RECT windowRect = {0,0,(SHORT)newScreenSize.x - 1,(SHORT)newScreenSize.y - 1};
	//SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE),TRUE,&windowRect);

	//// Sleep을 추가하여 콘솔 설정이 안정화될 시간을 줌
	//Sleep(100);
	//MoveWindow(consoleWindow,x,y,width,height,TRUE);
	//Sleep(100);

	// 엔진의 screenSize도 함께 업데이트
	//Engine::Get().ScreenSize() = Vector2(width/8,height/16);

	// 버퍼 재초기화
	//Engine::Get().InitializeScreenBuffers();

	// 콘솔 크기 변경
	//Engine::Get().ResizeConsole(x,y,width,height);
}