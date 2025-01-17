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
	// �ܼ� �ڵ� ��������
	console = GetConsoleWindow();
	if (console)
	{
		// �ܼ� â ��ġ �� ũ�� ����
		MoveWindow(console, consoleX, consoleY, consoleWidth, consoleHeight, TRUE);
	}

	// Ŀ�� ���߱�
	Engine::Get().SetCursorType(CursorType::NoCursor);

	// �� ���� �б�
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		std::cerr << "Failed to open map file: " << fileName << std::endl;
		__debugbreak();
	}

	int yPosition = 0; // y ��ǥ �ʱ�ȭ
	std::string line;

	// ���� �� ������ �б�
	while (std::getline(file, line))
	{
		std::vector<Actor*> row; // �� ���� ���� ����
		for (int xPosition = 0; xPosition < static_cast<int>(line.size()); ++xPosition)
		{
			char mapChar = line[xPosition];

			// ���ڿ� ���� ���� ����
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
				row.push_back(nullptr); // �� ����
			}
		}
		mapData.push_back(row);
		++yPosition; // ���� �ٷ� �̵�
	}

	// �� ũ�� ����
	mapHeight = static_cast<int>(mapData.size());
	mapWidth = mapData.empty() ? 0 : static_cast<int>(mapData[0].size());

    file.close();
}

void GameLevel::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// �÷��̾� ������Ʈ
	if (player)
	{
		player->Update(deltaTime);
	}

	// ������ Ŭ���� ������ ���� ���� ó��
	if (isGameClear)
	{
		// Ÿ�̸� Ŭ������ ��ü�� �ڵ� ����
		static Timer timer(0.1);
		timer.Update(deltaTime);
		if (!timer.IsTimeOut()) return;

		// Ŀ�� �̵�
		Engine::Get().SetCursorPosition(0, Engine::Get().ScreenSize().y);

		// �޼��� ���
		Log("Game Clear!\n");

		// ������ ����
		Sleep(2000);

		// ���� ���� ó��
		Engine::Get().QuitGame();
	}

}

void GameLevel::Draw()
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	float scaleX = static_cast<float>(screenWidth) / mapWidth;  // ���� ����
	float scaleY = static_cast<float>(screenHeight) / mapHeight; // ���� ����

	// ������ ����
	for (int y = 0; y < consoleHeight; ++y) {
		// mapData�� y �ε��� ���
		int mapY = static_cast<int>(consoleY + y * scaleY);
		if (mapY < 0 || mapY >= mapHeight) continue;  // mapData ���� �ʰ� �� ����

		for (int x = 0; x < consoleWidth; ++x) {
			// mapData�� x �ε��� ���
			int mapX = static_cast<int>(consoleX + x * scaleX);
			if (mapX < 0 || mapX >= mapWidth) continue;  // mapData ���� �ʰ� �� ����

			// mapData���� �ش� ��ġ�� Actor ��������
			Actor* actor = mapData[mapY][mapX];
			if (actor) {
				// �ֿܼ� ������
				//Engine::Get().SetCursorPosition(x, y);
				actor->Draw();
			}
		}
	}

	//for (int y = 0; y < mapHeight; ++y) {
	//	for (int x = 0; x < mapWidth; ++x) {
	//		// ȭ����� ������ ��ǥ ���
	//		int renderX = static_cast<int>((x - consoleX) * scaleX);
	//		int renderY = static_cast<int>((y - consoleY) * scaleY);

	//		// ȭ�� ���� ���Ե� ��쿡�� ������
	//		if (renderX >= 0 && renderX < consoleWidth &&
	//			renderY >= 0 && renderY < consoleHeight) {
	//			Actor* actor = mapData[y][x];
	//			if (actor) {
	//				actor->Draw();
	//			}
	//		}
	//	}
	//}

	// �ܼ�â ���� ���� ��(����)�� ������
	//for (int y = 0; y < consoleHeight; ++y) {
	//	int mapY = consoleY + y;
	//	if (mapY >= mapHeight || mapY < 0) break; // �� ��� �˻�

	//	for (int x = 0; x < consoleWidth; ++x) {
	//		int mapX = consoleX + x;
	//		if (mapX >= mapWidth || mapX < 0) break; // �� ��� �˻�

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

	//		// ȭ�� ���� ���Ե� ��쿡�� ���
	//		if (renderX >= 0 && renderX < mapWidth &&
	//			renderY >= 0 && renderY < mapHeight) {
	//			// ���� ������ ����
	//			Actor* actor = mapData[renderY][renderX];
	//			if (actor)
	//			{
	//				actor->Draw();
	//			}
	//		}
	//	}
	//}

	// �÷��̾� ��ġ �߾ӿ� ����
	if (player)
	{
		int playerX = consoleWidth / 2;
		int playerY = consoleHeight / 2;
		player->SetPosition(Vector2(playerX, playerY));
		player->Draw();
	}

#pragma region ���� �浹 �� ���� ó��
	// �� �׸���.
	//for (auto* actor : map)
	//{
	//	// �÷��̾� ��ġ Ȯ��
	//	if (actor->Position() == player->Position())
	//	{
	//		continue;
	//	}

	//	// �ڽ� ��ġ Ȯ��
	//	bool shouldDraw = true;
	//	/*for (auto* box : boxes)
	//	{
	//		if (actor->Position() == box->Position())
	//		{
	//			shouldDraw = false;
	//			break;
	//		}
	//	}*/

	//	// �� ���� �׸���
	//	if (shouldDraw)
	//	{
	//		actor->Draw();
	//	}
	//}

	////// Ÿ�� �׸���
	////for (auto* target : targets)
	////{
	////	// �÷��̾� ��ġ Ȯ��
	////	if (target->Position() == player->Position())
	////	{
	////		continue;
	////	}

	////	// �ڽ� ��ġ Ȯ��
	////	bool shouldDraw = true;
	////	for (auto* box : boxes)
	////	{
	////		if (target->Position() == box->Position())
	////		{
	////			shouldDraw = false;
	////			break;
	////		}
	////	}

	////	// Ÿ�� ���� �׸���
	////	if (shouldDraw)
	////	{
	////		target->Draw();
	////	}
	////}

	////// �ڽ� �׸���
	////for (auto* box : boxes)
	////{
	////	box->Draw();
	////}

	//// �÷��̾� �׸���
	//player->Draw();
#pragma endregion

}

bool GameLevel::CanPlayerMove(const Vector2& position)
{
	// ������ Ŭ����� ��� �ٷ� ����
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

