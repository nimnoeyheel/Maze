#pragma once

#include "Core.h"
#include "RTTI.h"
#include "Container/List.h"

class Actor; // 전방 선언 (forward Declaration)
class ENGINE_API Level : public RTTI
{
	// RTTI 정의
	RTTI_DECLARATIONS(Level, RTTI)

public:
	Level();
	virtual ~Level();

	// 액터 추가 함수
	void AddActor(Actor* newActor);
	
	// 삭제 요청이 된 액터를 정리하는 함수
	//void DestroyActor();
	void ProcessAddedAndDestroyActor();

	// 루프 처리 함수
	virtual void Update(float deltaTime);
	virtual void Draw();

protected:
	// 게임 공간에 배치되는 물체(액터) 배열
	List<Actor*> actors;
	
	// 추가 요청된 액터 처리
	Actor* addRequestedActor = nullptr;

	//// 게임 공간에 배치되는 물체(액터) 배열
	//Actor** actors;
	
	//// 저장할 수 있는 공간의 크기
	//int capacity;

	//// 액터의 수
	//int count;
};

