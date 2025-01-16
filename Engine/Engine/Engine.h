#pragma once

#include "Core.h"
#include "Math/Vector2.h"

// �Է� ó���� ���� ����ü
struct KeyState
{
	bool isKeyDown = false;  // ���� �����ӿ� Ű�� ���ȴ��� Ȯ��
	bool wasKeyDown = false; // ���� �����ӿ� Ű�� ���ȴ��� Ȯ��
};

// Ŀ���� ������ ������ �� ����� ������
enum class CursorType
{
	NoCursor,
	SolidCursor,
	NormalCursor
};


// Level, Actor Ŭ���� ���� ����
class Level;
class Actor;
class ENGINE_API Engine
{
public:
	Engine();
	virtual ~Engine();

	// ���� ���� �Լ�
	void Run();

	void LoadLevel(Level* newLevel);

	void AddActor(Actor* newActor);
	void DestroyActor(Actor* targetActor);

	// ȭ�� ��ǥ
	void SetCursorType(CursorType cursorType);
	void SetCursorPosition(const Vector2& position);
	void SetCursorPosition(int x, int y);

	// ȭ�� ũ�� ��ȯ �Լ�
	inline Vector2 ScreenSize() const { return screenSize; }

	// Ÿ�� ������ �ӵ� ���� �Լ�
	void SetTargetFrameRate(float targetFrameRate);

	// �Է� ���� �Լ�
	bool GetKey(int key);
	bool GetKeyDown(int key);
	bool GetKeyUp(int key);

	// ���� ���� �Լ�
	void QuitGame();

	// �̱��� ��ü 
	// ���� �Լ�
	static Engine& Get();

protected:
	void ProcessInput();			// �Է�ó��
	void Update(float deltaTime);	// Tick

	void Clear();					// ȭ�� �����
	void Draw();					// ������ - GPU���� / set data -> draw call.

	// ���� Ű ���¸� �����ϴ� �Լ�
	void SavePreiviouseKeyStates();

	// ���� ���� ����
	Level* mainLevel;

private:
	// Ÿ�� ������ ����(�ʴ� ������)
	float targetFrameRate = 60;

	// �� ������ �ð� ��(����: ��)
	float targetOneFrameTime = 0;

	// ���� ���� �� ������ ����
	bool quit;

	// Ű ���¸� �����ϴ� �迭
	KeyState keyState[255];

	// �̱��� ������ ���� ���� ���� ���� - ��� ������ ��ü ���������� �ʾƼ� �ʱ�ȭ �ؾ���
	static Engine* instance;

	// �������� ������Ʈ�ؾ� �ϴ��� ���θ� ��Ÿ���� ����
	bool shouldUpdate = true;

	// ȭ�� ũ��
	Vector2 screenSize;

	// ȭ�� ���� �� ����� ����(Buffer/Blob)
	char* emptyStringBuffer = nullptr;
};

