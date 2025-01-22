#pragma once

#include "Level/Level.h"
#include <vector>

struct MenuItems
{
	using OnSelected = void (*)(int);

	MenuItems(const char* text,OnSelected onSelected,int _stageNum)
		: onSelected(onSelected),stageNum(_stageNum)
	{
		size_t length = strlen(text);
		menuText = new char[length + 1];
		strcpy_s(menuText,length + 1,text);
	}

	~MenuItems()
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

class FailedLevel : public Level,public std::enable_shared_from_this<FailedLevel*>
{
	RTTI_DECLARATIONS(FailedLevel,Level)

public:
	FailedLevel(int _stageNum);
	~FailedLevel();

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

private:
	int currentIndex = 0;
	Color selectedColor = Color::Green;
	Color unselectedColor = Color::White;

	std::vector<MenuItems*> menuItems;
	int length = 0;

	int stageNum;
};

