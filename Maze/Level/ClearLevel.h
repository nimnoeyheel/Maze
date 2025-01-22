#pragma once

#include "Level/Level.h"
#include <vector>

struct MenuItem
{
	//using OnSelected = std::function<void(int)>;
	using OnSelected = void (*)(int);

	MenuItem(const char* text,OnSelected onSelected,int _stageNum)
		: onSelected(onSelected),stageNum(_stageNum)
	{
		size_t length = strlen(text);
		menuText = new char[length + 1];
		strcpy_s(menuText,length + 1,text);
	}

	//template <typename T>
	//MenuItem(const char* text,T* instance,void (T::*method)(int),int _stageNum)
	//	: stageNum(_stageNum) // stageNum 초기화
	//{
	//	size_t length = strlen(text);
	//	menuText = new char[length + 1];
	//	strcpy_s(menuText,length + 1,text);

	//	onSelected = [instance,method](int stageNum) {
	//		(instance->*method)(stageNum);
	//	};
	//}

	~MenuItem()
	{
		if(menuText)
		{
			delete[] menuText;
		}
	}

	void Execute() const
	{
		if(onSelected)
		{
			onSelected(stageNum); // 함수 포인터 호출
		}
	}

	char* menuText;
	OnSelected onSelected;
	int stageNum;
};

class ClearLevel : public Level, public std::enable_shared_from_this<ClearLevel*>
{
	RTTI_DECLARATIONS(ClearLevel,Level)

public:
	ClearLevel(int _stageNum,int _score,int _playTime);
	~ClearLevel();

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

private:
	int currentIndex = 0;
	Color selectedColor = Color::Green;
	Color unselectedColor = Color::White;

	std::vector<MenuItem*> menuItems;
	int length = 0;

	int stageNum;
	int score;
	int playTime;
};