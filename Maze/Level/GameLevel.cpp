#define NOMINMAX

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
	// 콘솔 핸들 가져오기
	console = GetConsoleWindow();
	if (console)
	{
		// 콘솔 창 위치 및 크기 설정
		MoveWindow(console, consoleX, consoleY, consoleWidth, consoleHeight, TRUE);
	}

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
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	float scaleX = static_cast<float>(screenWidth) / mapWidth;  // 가로 비율
	float scaleY = static_cast<float>(screenHeight) / mapHeight; // 세로 비율

	// 렌더링 시작
	for (int y = 0; y < consoleHeight; ++y) {
		// mapData의 y 인덱스 계산
		int mapY = static_cast<int>(consoleY + y * scaleY);
		if (mapY < 0 || mapY >= mapHeight) continue;  // mapData 범위 초과 시 무시

		for (int x = 0; x < consoleWidth; ++x) {
			// mapData의 x 인덱스 계산
			int mapX = static_cast<int>(consoleX + x * scaleX);
			if (mapX < 0 || mapX >= mapWidth) continue;  // mapData 범위 초과 시 무시

			// mapData에서 해당 위치의 Actor 가져오기
			Actor* actor = mapData[mapY][mapX];
			if (actor) {
				// 콘솔에 렌더링
				//Engine::Get().SetCursorPosition(x, y);
				actor->Draw();
			}
		}
	}

	//for (int y = 0; y < mapHeight; ++y) {
	//	for (int x = 0; x < mapWidth; ++x) {
	//		// 화면상의 렌더링 좌표 계산
	//		int renderX = static_cast<int>((x - consoleX) * scaleX);
	//		int renderY = static_cast<int>((y - consoleY) * scaleY);

	//		// 화면 내에 포함된 경우에만 렌더링
	//		if (renderX >= 0 && renderX < consoleWidth &&
	//			renderY >= 0 && renderY < consoleHeight) {
	//			Actor* actor = mapData[y][x];
	//			if (actor) {
	//				actor->Draw();
	//			}
	//		}
	//	}
	//}

	// 콘솔창 범위 내의 맵(액터)만 렌더링
	//for (int y = 0; y < consoleHeight; ++y) {
	//	int mapY = consoleY + y;
	//	if (mapY >= mapHeight || mapY < 0) break; // 맵 경계 검사

	//	for (int x = 0; x < consoleWidth; ++x) {
	//		int mapX = consoleX + x;
	//		if (mapX >= mapWidth || mapX < 0) break; // 맵 경계 검사

	//		Actor* actor = mapData[mapY][mapX];
	//		if (actor)
	//		{
	//			actor->Draw();
	//		}
	//	}
	//}

	//for (int y = 0; y < mapHeight; ++y) {
	//	for (int x = 0; x < mapWidth; ++x) {
	//		int renderY = y - consoleY;
	//		int renderX = x - consoleX;

	//		// 화면 내에 포함된 경우에만 출력
	//		if (renderX >= 0 && renderX < mapWidth &&
	//			renderY >= 0 && renderY < mapHeight) {
	//			// 실제 렌더링 수행
	//			Actor* actor = mapData[renderY][renderX];
	//			if (actor)
	//			{
	//				actor->Draw();
	//			}
	//		}
	//	}
	//}

	// 플레이어 위치 중앙에 고정
	if (player)
	{
		int playerX = consoleWidth / 2;
		int playerY = consoleHeight / 2;
		player->SetPosition(Vector2(playerX, playerY));
		player->Draw();
	}

#pragma region 액터 충돌 시 렌더 처리
	// 맵 그리기.
	//for (auto* actor : map)
	//{
	//	// 플레이어 위치 확인
	//	if (actor->Position() == player->Position())
	//	{
	//		continue;
	//	}

	//	// 박스 위치 확인
	//	bool shouldDraw = true;
	//	/*for (auto* box : boxes)
	//	{
	//		if (actor->Position() == box->Position())
	//		{
	//			shouldDraw = false;
	//			break;
	//		}
	//	}*/

	//	// 맵 액터 그리기
	//	if (shouldDraw)
	//	{
	//		actor->Draw();
	//	}
	//}

	////// 타켓 그리기
	////for (auto* target : targets)
	////{
	////	// 플레이어 위치 확인
	////	if (target->Position() == player->Position())
	////	{
	////		continue;
	////	}

	////	// 박스 위치 확인
	////	bool shouldDraw = true;
	////	for (auto* box : boxes)
	////	{
	////		if (target->Position() == box->Position())
	////		{
	////			shouldDraw = false;
	////			break;
	////		}
	////	}

	////	// 타켓 액터 그리기
	////	if (shouldDraw)
	////	{
	////		target->Draw();
	////	}
	////}

	////// 박스 그리기
	////for (auto* box : boxes)
	////{
	////	box->Draw();
	////}

	//// 플레이어 그리기
	//player->Draw();
#pragma endregion

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
	int screenWidth = GetSystemMetrics(SM_CXSCREEN) - consoleX / 4;
	int screenHeight = GetSystemMetrics(SM_CYSCREEN) - consoleY / 2;

	if (consoleX < 0) consoleX = 0;
	if (consoleY < 0) consoleY = 0;
	if (consoleX > screenWidth) consoleX = screenWidth;
	if (consoleY > screenHeight) consoleY = screenHeight;

	MoveWindow(console, consoleX, consoleY, consoleWidth, consoleHeight, TRUE);
}

