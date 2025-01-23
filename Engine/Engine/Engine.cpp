#include "PreCompiledHeader.h"

#include "Engine.h"
#include <Windows.h>
#include <iostream>

#include "Level/Level.h"
#include "Actor/Actor.h"

#include <time.h>

#include "Render/ScreenBuffer.h"

// 콘솔 창 메시지 콜백 함수.
BOOL WINAPI MessageProcessor(DWORD message)
{
	switch (message)
	{
	case CTRL_CLOSE_EVENT:
		Engine::Get().QuitGame();
		return true;

	default:
		return false;
	}
}

// 스태틱 변수 초기화.
Engine* Engine::instance = nullptr;

Engine::Engine()
	: quit(false), mainLevel(nullptr), screenSize(0,0)
{
	// 랜덤 시드 설정.
	srand((unsigned int)time(nullptr));

	// 싱글톤 객체 설정.
	instance = this;

	// 기본 타겟 프레임 속도 설정.
	SetTargetFrameRate(60.0f);

	// 콘솔 창 초기화 (크기와 위치 설정)
	InitializeConsole(800,500);

	// 화면 버퍼 초기화.
	InitializeScreenBuffers();

	// 스왑 버퍼.
	Present();

	// 콘솔 창 이벤트 콜백 함수 등록.
	SetConsoleCtrlHandler(MessageProcessor, true);
}

Engine::~Engine()
{
	// 메인 레벨 메모리 해제.
	if (mainLevel != nullptr)
	{
		delete mainLevel;
	}

	// 클리어 버퍼 삭제.
	delete[] imageBuffer;

	// 화면 버퍼 삭제.
	delete renderTargets[0];
	delete renderTargets[1];
}

void Engine::Run()
{
	// 시작 타임 스탬프 저장.
	// timeGetTime 함수는 밀리세컨드(1/1000초) 단위.
	//unsigned long currentTime = timeGetTime();
	//unsigned long previousTime = 0;

	// CPU 시계 사용.
	// 시스템 시계 -> 고해상도 카운터. (10000000).
	// 메인보드에 시계가 있음.
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	//std::cout << "Frequency: " << frequency.QuadPart << "\n";

	// 시작 시간 및 이전 시간을 위한 변수.
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	int64_t currentTime = time.QuadPart;
	int64_t previousTime = currentTime;

	// 프레임 제한.
	//float targetFrameRate = 90.0f;

	// 한 프레임 시간 계산.
	//float targetOneFrameTime = 1.0f / targetFrameRate;

	// Game-Loop.
	while (true)
	{
		// 종료 조건.
		if (quit)
		{
			break;
		}

		// 현재 프레임 시간 저장.
		//time = timeGetTime();
		QueryPerformanceCounter(&time);
		currentTime = time.QuadPart;

		// 프레임 시간 계산.
		float deltaTime = static_cast<float>(currentTime - previousTime) /
			static_cast<float>(frequency.QuadPart);

		// 한 프레임 시간 계산.
		//float targetOneFrameTime = 1.0f / targetFrameRate;

		// 프레임 확인.
		if (deltaTime >= targetOneFrameTime)
		{
			// 입력 처리 (현재 키의 눌림 상태 확인).
			ProcessInput();

			// 업데이트 가능한 상태에서만 프레임 업데이트 처리.
			if (shouldUpdate)
			{
				Update(deltaTime);
				Draw(); // @Todo : 프레임마다 Draw 함수 호출하면서 Draw함수 내부에서 백버퍼 프론트버퍼 교환해주고 있음
			}

			// 키 상태 저장.
			SavePreviouseKeyStates();

			// 이전 프레임 시간 저장.
			previousTime = currentTime;

			// 액터 정리 (삭제 요청된 액터들 정리).
			if (mainLevel)
			{
				//mainLevel->DestroyActor();
				mainLevel->ProcessAddedAndDestroyedActor();
			}

			// 버퍼 스왑이 필요한 경우에만 Present() 호출
			if(shouldPresent)
			{
				Present();
				shouldPresent = false;
			}

			// 프레임 활성화.
			shouldUpdate = true;
		}
	}
}

void Engine::LoadLevel(Level* newLevel)
{
	// 기존 레벨이 있다면 삭제 후 교체.
	// 메인 레벨 설정.
	mainLevel = newLevel;

	//// 새로운 레벨의 버퍼 크기에 맞춰 화면 버퍼 초기화
	//if(mainLevel)
	//{
	//	// 레벨에서 설정된 화면 크기로 버퍼 재초기화
	//	screenSize.x = mainLevel->GetBufferWidth();
	//	screenSize.y = mainLevel->GetBufferHeight();

	//	// 기존 이미지 버퍼 삭제
	//	delete[] imageBuffer;

	//	// 새 버퍼 크기에 맞춰 이미지 버퍼 재할당
	//	imageBuffer = new CHAR_INFO[(screenSize.x + 1) * screenSize.y + 1];
	//	ClearImageBuffer();

	//	// 렌더 타겟 버퍼 크기 업데이트
	//	COORD newSize = {(short)screenSize.x,(short)screenSize.y};
	//	renderTargets[0]->size = newSize;
	//	renderTargets[1]->size = newSize;
	//}
}

void Engine::AddActor(Actor* newActor)
{
	// 예외 처리.
	if (mainLevel == nullptr)
	{
		return;
	}

	// 레벨에 액터 추가.
	shouldUpdate = false;
	mainLevel->AddActor(newActor);
}

void Engine::DestroyActor(Actor* targetActor)
{
	// 예외 처리.
	if (mainLevel == nullptr)
	{
		return;
	}

	// 레벨에 액터 추가.
	shouldUpdate = false;
	targetActor->Destroy();
}

void Engine::SetCursorType(CursorType cursorType)
{
	GetRenderer()->SetCursorType(cursorType);
}

void Engine::Draw(const Vector2& position, const char* image, Color color)
{
	for (int ix = 0; ix < (int)strlen(image); ++ix)
	{
		int index = (position.y * (screenSize.x)) + position.x + ix;

		// 인덱스 경계 확인
		if(index < 0 || index >= (screenSize.x * screenSize.y)) continue; // 범위를 초과한 경우 무시

		imageBuffer[index].Char.AsciiChar = image[ix];
		imageBuffer[index].Attributes = (unsigned long)color;
	}
}

void Engine::SetTargetFrameRate(float targetFrameRate)
{
	this->targetFrameRate = targetFrameRate;
	targetOneFrameTime = 1.0f / targetFrameRate;
}

bool Engine::GetKey(int key)
{
	return keyState[key].isKeyDown;
}

bool Engine::GetKeyDown(int key)
{
	return keyState[key].isKeyDown && !keyState[key].wasKeyDown;
}

bool Engine::GetKeyUp(int key)
{
	return !keyState[key].isKeyDown && keyState[key].wasKeyDown;
}

void Engine::QuitGame()
{
	// 종료 플래그 설정.
	quit = true;
}

Engine& Engine::Get()
{
	// 싱글톤 객체 반환.
	return *instance;
}

void Engine::ProcessInput()
{
	for (int ix = 0; ix < 255; ++ix)
	{
		keyState[ix].isKeyDown = (GetAsyncKeyState(ix) & 0x8000) ? true : false;
	}
}

void Engine::Update(float deltaTime)
{
	// 레벨 업데이트.
	if (mainLevel != nullptr)
	{
		mainLevel->Update(deltaTime);
	}
}

void Engine::Clear()
{
	ClearImageBuffer();
	GetRenderer()->Clear();
}

void Engine::Draw()
{
	// 화면 지우기.
	Clear();

	// 레벨 그리기.
	if (mainLevel != nullptr)
	{
		mainLevel->Draw();
	}

	// 백버퍼에 데이터 쓰기.
	GetRenderer()->Draw(imageBuffer);

	// 버퍼 스왑 플래그 설정
	shouldPresent = true;

	// 프론트<->백 버퍼 교환. //@Todo : 프레임마다 호출해주고 있음 수정 필요
	//Present();
}

void Engine::Present()
{
	// Swap Buffer.
	//SetConsoleActiveScreenBuffer(GetRenderer()->buffer); //@Todo : 렌더의 버퍼 값을 InitializeConsole() 내부에 있는 버퍼랑 동기화 필요할 듯 
	
	SetConsoleActiveScreenBuffer(renderTargets[currentRenderTargetIndex]->buffer);

	currentRenderTargetIndex = 1 - currentRenderTargetIndex;
}

void Engine::SavePreviouseKeyStates()
{
	for (int ix = 0; ix < 255; ++ix)
	{
		keyState[ix].wasKeyDown = keyState[ix].isKeyDown;
	}
}

void Engine::ClearImageBuffer()
{
	// 버퍼 덮어쓰기.
	for (int y = 0; y < screenSize.y; ++y)
	{
		// 버퍼 덮어쓰기.
		for (int x = 0; x < screenSize.x + 1; ++x)
		{
			auto& buffer = imageBuffer[(y * (screenSize.x + 1)) + x];
			buffer.Char.AsciiChar = ' ';
			buffer.Attributes = 0;
		}

		// 각 줄 끝에 개행 문자 추가.
		auto& buffer = imageBuffer[(y * (screenSize.x + 1)) + screenSize.x];
		buffer.Char.AsciiChar = '\n';
		buffer.Attributes = 0;
	}

	// 마지막에 널 문자 추가.
	auto& buffer = imageBuffer[(screenSize.x + 1) * screenSize.y];
	buffer.Char.AsciiChar = '\0';
	buffer.Attributes = 0;
}

void Engine::InitializeConsole(int pixelWidth,int pixelHeight)
{
	// 새로운 콘솔 창 생성
	AllocConsole();

	HWND consoleWindow = GetConsoleWindow();
	if(consoleWindow == NULL) return;

	// 문자 셀 크기를 명시적으로 설정
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 8;
	cfi.dwFontSize.Y = 16;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName,L"Terminal");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE),FALSE,&cfi);

	// screenSize를 먼저 업데이트 (글자 크기로 나눈 값)
	screenSize.x = pixelWidth / cfi.dwFontSize.X;
	screenSize.y = pixelHeight / cfi.dwFontSize.Y;

	// 콘솔 버퍼 크기 설정
	COORD bufferSize = {(SHORT)(screenSize.x),(SHORT)(screenSize.y)};
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),bufferSize);

	// 콘솔 창 크기 설정
	SMALL_RECT rect = {0,0,bufferSize.X - 1,bufferSize.Y - 1};
	//SMALL_RECT rect = {0,0,(SHORT)screenSize.x - 1,(SHORT)screenSize.y - 1};
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE),TRUE,&rect);

	// 창 위치를 화면 중앙으로 이동
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int x = (screenWidth - pixelWidth) / 2;
	int y = (screenHeight - pixelHeight) / 2;

	Sleep(100);
	MoveWindow(consoleWindow,x,y,pixelWidth,pixelHeight,TRUE);
	Sleep(100);

	// screenSize 업데이트
	screenSize.x = bufferSize.X;
	screenSize.y = bufferSize.Y;
}

void Engine::InitializeScreenBuffers()
{
	// 버퍼 초기화 로직을 별도 함수로 분리
	imageBuffer = new CHAR_INFO[(screenSize.x + 1) * screenSize.y + 1];
	ClearImageBuffer();

	COORD size = {(short)screenSize.x,(short)screenSize.y};
	renderTargets[0] = new ScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE),size);
	renderTargets[1] = new ScreenBuffer(size);
}