#pragma once

#include "Level/Level.h"
#include <vector>

struct MenuItem
{
	using OnSelected = void (*)();

	MenuItem(const char* text, OnSelected onSelected)
	{
		size_t length = strlen(text);
		menuText = new char[length + 1];
		strcpy_s(menuText, length + 1, text);

		this->onSelected = onSelected;
	}

	~MenuItem() {}

	char* menuText;
	OnSelected onSelected;
};

class StartLevel : public Level
{
	RTTI_DECLARATIONS(StartLevel, Level)

public:
	StartLevel();
	~StartLevel();

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

private:
	int currentIndex = 0;
	Color selectedColor = Color::Green;
	Color unselectedColor = Color::White;

	std::vector<MenuItem*> menuItems;
	int length = 0;
};