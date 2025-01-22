#include "PreCompiledHeader.h"
#include "DrawableActor.h"
#include "Engine/Engine.h"

DrawableActor::DrawableActor(const char* image)
	: Actor()/*, image(image)*/
{
	// ���� ���� ���ڿ� ����.
	auto length = strlen(image) + 1;
	this->image = new char[length];
	memset(this->image,0,length); // �ʱ�ȭ
	strcpy_s(this->image, length, image);

	// �ʺ� ����.
	width = (int)strlen(image);
}

DrawableActor::~DrawableActor()
{
	delete[] image;
	image = nullptr;
}

void DrawableActor::Draw()
{
	Super::Draw();

	Engine::Get().Draw(position, image, color);
}

void DrawableActor::SetPosition(const Vector2& newPosition)
{
	//// ������ ��ġ�� ���� �����.
	//Engine::Get().SetCursorPosition(position);
	//Log(" ");

	// ��ġ�� ���� �ű��.
	Super::SetPosition(newPosition);
}

bool DrawableActor::Intersect(const DrawableActor& other)
{
	// AABB(Axis Aligned Bounding Box).

	// �� x��ǥ �ּ�/�ִ�.
	int min = position.x;
	int max = position.x + width;

	// �ٸ� ������ x��ǥ �ּ�/�ִ�.
	int otherMin = other.position.x;
	int otherMax = other.position.x + other.width;

	// �ٸ� ������ ���� �� ��ġ�� �� ������ �� ��ġ�� ����� �浹 ����.
	if (otherMin > max)
	{
		return false;
	}

	// �ٸ� ������ ������ �� ��ġ�� �� ���� �� ��ġ���� ������ �浹 ����.
	if (otherMax < min)
	{
		return false;
	}

	// ���� �� ��찡 �ƴ϶�� (x��ǥ�� ���� ��ħ), y��ġ ��.
	return position.y == other.position.y;
}