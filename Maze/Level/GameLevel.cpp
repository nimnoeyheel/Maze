#include "GameLevel.h"
#include "Engine/Engine.h"
#include "Actor/Player.h"
#include "Engine/Timer.h"
#include "Actor/Wall.h"

GameLevel::GameLevel(const char* mapFileName, int startX, int startY, int width, int height)
	: mapFileName(mapFileName), consoleX(startX), consoleY(startY), consoleWidth(width), consoleHeight(height)
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

	// �� ���� �ҷ��� ���� �ε�

	// ���� �б�
	FILE* file = nullptr;
	fopen_s(&file, mapFileName, "rb");

	// ���� ó��
	if (file == nullptr)
	{
		std::cout << "�� ���� ���� ����\n";
		__debugbreak();
		return;
	}

	// ���� �б�
	// �� ��ġ�� �̵�: ���� �� ��ġ �������� ������ 0��ŭ ��
	fseek(file, 0, SEEK_END);

	// �̵��� ��ġ�� FP ��������
	size_t readSize = ftell(file);

	// FP ����ġ / SEEK_SET: ù ��ġ
	//fseek(file, 0, SEEK_SET);
	rewind(file);

	// ���� �о ���ۿ� ���
	char* buffer = new char[readSize + 1];
	size_t bytesRead = fread(buffer, 1, readSize, file);

	if (readSize != bytesRead)
	{
		std::cout << "�о�� ũ�Ⱑ �ٸ�\n";
		__debugbreak();
		return;
	}

	buffer[readSize] = '\0';

	// ���� ���� �� ����� �ε���
	int index = 0;

	// ��ǥ ����� ���� ���� ����
	int xPosition = 0;
	int yPosition = 0;

	// �ؼ� (�Ľ�-Parcing)
	while (index < (int)bytesRead)
	{
		// �� ���ھ� �б�
		char mapChar = buffer[index++];

		// ���� ������ ��� ó��
		if (mapChar == '\n')
		{
			++yPosition;
			xPosition = 0;
			continue;
		}

		// �� ���ڰ� 1�̸� Wall ���� ����
		if (mapChar == '1')
		{
			Wall* wall = new Wall(Vector2(xPosition, yPosition));
			actors.PushBack(wall);
			map.PushBack(wall);
		}
		// �� ���ڰ� p�̸� �÷��̾� ���� ����
		else if (mapChar == 'p')
		{
			player = new Player(Vector2(xPosition, yPosition), this);
			actors.PushBack(player);
		}

		++xPosition;
	}

	// ���� ����
	delete[] buffer;

	// ���� �ݱ�
	fclose(file);
}

void GameLevel::Update(float deltaTime)
{
	Super::Update(deltaTime);

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
	// �� �׸���.
	for (auto* actor : map)
	{
		// �÷��̾� ��ġ Ȯ��
		if (actor->Position() == player->Position())
		{
			continue;
		}

		//// �ڽ� ��ġ Ȯ��
		//bool shouldDraw = true;
		//for (auto* box : boxes)
		//{
		//	if (actor->Position() == box->Position())
		//	{
		//		shouldDraw = false;
		//		break;
		//	}
		//}

		//// �� ���� �׸���
		//if (shouldDraw)
		//{
		//	actor->Draw();
		//}
	}

	//// Ÿ�� �׸���
	//for (auto* target : targets)
	//{
	//	// �÷��̾� ��ġ Ȯ��
	//	if (target->Position() == player->Position())
	//	{
	//		continue;
	//	}

	//	// �ڽ� ��ġ Ȯ��
	//	bool shouldDraw = true;
	//	for (auto* box : boxes)
	//	{
	//		if (target->Position() == box->Position())
	//		{
	//			shouldDraw = false;
	//			break;
	//		}
	//	}

	//	// Ÿ�� ���� �׸���
	//	if (shouldDraw)
	//	{
	//		target->Draw();
	//	}
	//}

	//// �ڽ� �׸���
	//for (auto* box : boxes)
	//{
	//	box->Draw();
	//}

	// �÷��̾� �׸���
	player->Draw();
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
