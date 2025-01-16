#include "GameLevel.h"
#include "Engine/Engine.h"
#include "Actor/Player.h"
#include "Engine/Timer.h"
#include "Actor/Wall.h"

GameLevel::GameLevel(const char* mapFileName, int startX, int startY, int width, int height)
	: mapFileName(mapFileName), consoleX(startX), consoleY(startY), consoleWidth(width), consoleHeight(height)
{
	// 콘솔 핸들 가져오기
	console = GetConsoleWindow();
	if (console)
	{
		// 콘솔 창 위치 및 크기 설정
		MoveWindow(console, consoleX, consoleY, consoleWidth, consoleHeight, TRUE);
	}

	// 커서 감추기
	Engine::Get().SetCursorType(CursorType::NoCursor);

	// 맵 파일 불러와 레벨 로드

	// 파일 읽기
	FILE* file = nullptr;
	fopen_s(&file, mapFileName, "rb");

	// 파일 처리
	if (file == nullptr)
	{
		std::cout << "맵 파일 열기 실패\n";
		__debugbreak();
		return;
	}

	// 파일 읽기
	// 끝 위치로 이동: 파일 끝 위치 기준으로 오프셋 0만큼 감
	fseek(file, 0, SEEK_END);

	// 이동한 위치의 FP 가져오기
	size_t readSize = ftell(file);

	// FP 원위치 / SEEK_SET: 첫 위치
	//fseek(file, 0, SEEK_SET);
	rewind(file);

	// 파일 읽어서 버퍼에 담기
	char* buffer = new char[readSize + 1];
	size_t bytesRead = fread(buffer, 1, readSize, file);

	if (readSize != bytesRead)
	{
		std::cout << "읽어온 크기가 다름\n";
		__debugbreak();
		return;
	}

	buffer[readSize] = '\0';

	// 파일 읽을 때 사용할 인덱스
	int index = 0;

	// 좌표 계산을 위한 변수 선언
	int xPosition = 0;
	int yPosition = 0;

	// 해석 (파싱-Parcing)
	while (index < (int)bytesRead)
	{
		// 한 문자씩 읽기
		char mapChar = buffer[index++];

		// 개행 문자인 경우 처리
		if (mapChar == '\n')
		{
			++yPosition;
			xPosition = 0;
			continue;
		}

		// 맵 문자가 1이면 Wall 액터 생성
		if (mapChar == '1')
		{
			Wall* wall = new Wall(Vector2(xPosition, yPosition));
			actors.PushBack(wall);
			map.PushBack(wall);
		}
		// 맵 문자가 p이면 플레이어 액터 생성
		else if (mapChar == 'p')
		{
			player = new Player(Vector2(xPosition, yPosition), this);
			actors.PushBack(player);
		}

		++xPosition;
	}

	// 버퍼 삭제
	delete[] buffer;

	// 파일 닫기
	fclose(file);
}

void GameLevel::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// 게임이 클리어 됐으면 게임 종료 처리
	if (isGameClear)
	{
		// 타이머 클래스의 객체로 코드 정리
		static Timer timer(0.1);
		timer.Update(deltaTime);
		if (!timer.IsTimeOut()) return;

		// 커서 이동
		Engine::Get().SetCursorPosition(0, Engine::Get().ScreenSize().y);

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
	// 맵 그리기.
	for (auto* actor : map)
	{
		// 플레이어 위치 확인
		if (actor->Position() == player->Position())
		{
			continue;
		}

		//// 박스 위치 확인
		//bool shouldDraw = true;
		//for (auto* box : boxes)
		//{
		//	if (actor->Position() == box->Position())
		//	{
		//		shouldDraw = false;
		//		break;
		//	}
		//}

		//// 맵 액터 그리기
		//if (shouldDraw)
		//{
		//	actor->Draw();
		//}
	}

	//// 타켓 그리기
	//for (auto* target : targets)
	//{
	//	// 플레이어 위치 확인
	//	if (target->Position() == player->Position())
	//	{
	//		continue;
	//	}

	//	// 박스 위치 확인
	//	bool shouldDraw = true;
	//	for (auto* box : boxes)
	//	{
	//		if (target->Position() == box->Position())
	//		{
	//			shouldDraw = false;
	//			break;
	//		}
	//	}

	//	// 타켓 액터 그리기
	//	if (shouldDraw)
	//	{
	//		target->Draw();
	//	}
	//}

	//// 박스 그리기
	//for (auto* box : boxes)
	//{
	//	box->Draw();
	//}

	// 플레이어 그리기
	player->Draw();
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
	int screenWidth = GetSystemMetrics(SM_CXSCREEN) - consoleX/4;
	int screenHeight = GetSystemMetrics(SM_CYSCREEN) - consoleY/2;

	if (consoleX < 0) consoleX = 0;
	if (consoleY < 0) consoleY = 0;
	if (consoleX > screenWidth) consoleX = screenWidth;
	if (consoleY > screenHeight) consoleY = screenHeight;
	{
		MoveWindow(console, consoleX, consoleY, consoleWidth, consoleHeight, TRUE);
	}
}
