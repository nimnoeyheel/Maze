#include "PreCompiledHeader.h"
#include "Level.h"
#include "Actor/Actor.h" // 속성에 추가 포함 디렉터리 설정 $(ProjectDir)\;
//#include "../Actor/Actor.h"

//#include <iostream>

Level::Level()
	//: actors(nullptr), capacity(4), count(0)
{
	// 동적 배열 생성
	//actors = new Actor * [capacity];

	// actors를 null로 초기화
	//memset(actors, 0, sizeof(size_t) * capacity);
}

Level::~Level()
{
	// 메모리 해제
	//for (int i = 0; i < actors.Size; ++i) delete actors[i];

	// 동적 배열 해제
	//delete actors[i];

	for (Actor* actor : actors)
	{
		delete actor;
	}
}

void Level::AddActor(Actor* newActor)
{
	//actors.PushBack(newActor);
	addRequestedActor = newActor;

	//// 현재 할당된 공간이 충분한지 확인
	//if (count == capacity)
	//{
	//	// 공간 추가 할당 : 공간을 얼마나 할당할 것인가
	//	int newCapacity = capacity * 2;

	//	// 임시 공간 : 얼마나 최적화를 할 수 있나
	//	Actor** temp = new Actor * [newCapacity]; // 8

	//	// 기존 값 복사
	//	// loop -> FMemory::Copy, memcpy
	//	memcpy(temp, actors, sizeof(size_t) * capacity); // 4, Move 가능

	//	// 기존 배열 삭제
	//	delete[] actors;

	//	// 배열 교체
	//	actors = temp;

	//	// 크기 변경
	//	capacity = newCapacity;
	//}

	//// 액터 추가
	//actors[count] = newActor;
	//++count;
}

//void Level::DestroyActor()
//{
//	// 액터 순회 후 삭제 요청된 액터를 처리
//	for (int idx = 0; idx < actors.Size();)
//	{
//		if (actors[idx]->isExpired)
//		{
//			delete actors[idx];
//			actors[idx] = nullptr;
//			actors.Erase(idx);
//			continue;
//		}
//
//		++idx;
//	}
//}

void Level::ProcessAddedAndDestroyActor()
{
	//액터 순회 후 삭제 요청된 액터를 처리
	for (int idx = 0; idx < actors.Size();)
	{
		if (actors[idx]->isExpired)
		{
			delete actors[idx];
			actors[idx] = nullptr;
			actors.Erase(idx);
			continue;
		}

		++idx;
	}

	// 추가 요청된 액터 처리
	if (addRequestedActor != nullptr)
	{
		actors.PushBack(addRequestedActor);
		addRequestedActor = nullptr;
	}
}

void Level::Update(float deltaTime)
{
	// 레벨에 포함된 액터를 순회하면서 Update 함수 호출
	for (Actor* actor : actors)
	{
		// 액터가 비활성 상태이거나, 삭제 요청된 경우 건너뛰기
		if (!actor->isActive || actor->isExpired) continue;

		actor->Update(deltaTime);
	}
	
	/*for (int i = 0; i < actors.size(); ++i)
	{
		actors[i]->Update(deltaTime);
	}*/
}

void Level::Draw()
{
	// 레벨에 포함된 액터를 순회하면서 Draw 함수 호출
	for (Actor* actor : actors)
	{
		// 액터가 비활성 상태이거나, 삭제 요청된 경우 건너뛰기
		if (!actor->isActive || actor->isExpired) continue;

		actor->Draw();
	}

	/*for (int i = 0; i < actors.size(); ++i)
	{
		actors[i]->Draw();
	}*/
}
