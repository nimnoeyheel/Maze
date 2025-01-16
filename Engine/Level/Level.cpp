#include "PreCompiledHeader.h"
#include "Level.h"
#include "Actor/Actor.h" // �Ӽ��� �߰� ���� ���͸� ���� $(ProjectDir)\;
//#include "../Actor/Actor.h"

//#include <iostream>

Level::Level()
	//: actors(nullptr), capacity(4), count(0)
{
	// ���� �迭 ����
	//actors = new Actor * [capacity];

	// actors�� null�� �ʱ�ȭ
	//memset(actors, 0, sizeof(size_t) * capacity);
}

Level::~Level()
{
	// �޸� ����
	//for (int i = 0; i < actors.Size; ++i) delete actors[i];

	// ���� �迭 ����
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

	//// ���� �Ҵ�� ������ ������� Ȯ��
	//if (count == capacity)
	//{
	//	// ���� �߰� �Ҵ� : ������ �󸶳� �Ҵ��� ���ΰ�
	//	int newCapacity = capacity * 2;

	//	// �ӽ� ���� : �󸶳� ����ȭ�� �� �� �ֳ�
	//	Actor** temp = new Actor * [newCapacity]; // 8

	//	// ���� �� ����
	//	// loop -> FMemory::Copy, memcpy
	//	memcpy(temp, actors, sizeof(size_t) * capacity); // 4, Move ����

	//	// ���� �迭 ����
	//	delete[] actors;

	//	// �迭 ��ü
	//	actors = temp;

	//	// ũ�� ����
	//	capacity = newCapacity;
	//}

	//// ���� �߰�
	//actors[count] = newActor;
	//++count;
}

//void Level::DestroyActor()
//{
//	// ���� ��ȸ �� ���� ��û�� ���͸� ó��
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
	//���� ��ȸ �� ���� ��û�� ���͸� ó��
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

	// �߰� ��û�� ���� ó��
	if (addRequestedActor != nullptr)
	{
		actors.PushBack(addRequestedActor);
		addRequestedActor = nullptr;
	}
}

void Level::Update(float deltaTime)
{
	// ������ ���Ե� ���͸� ��ȸ�ϸ鼭 Update �Լ� ȣ��
	for (Actor* actor : actors)
	{
		// ���Ͱ� ��Ȱ�� �����̰ų�, ���� ��û�� ��� �ǳʶٱ�
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
	// ������ ���Ե� ���͸� ��ȸ�ϸ鼭 Draw �Լ� ȣ��
	for (Actor* actor : actors)
	{
		// ���Ͱ� ��Ȱ�� �����̰ų�, ���� ��û�� ��� �ǳʶٱ�
		if (!actor->isActive || actor->isExpired) continue;

		actor->Draw();
	}

	/*for (int i = 0; i < actors.size(); ++i)
	{
		actors[i]->Draw();
	}*/
}
