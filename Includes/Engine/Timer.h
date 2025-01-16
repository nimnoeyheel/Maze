#pragma once

#include "Core.h"

// �ʽð� Ŭ����
class ENGINE_API Timer
{
public:
	// �����ڿ��� Ÿ�̸��� ��ǥ �ð� ����
	Timer(float targetTime);

	void Update(float deltaTime);

	// Ÿ�̸� �ʱ�ȭ
	void Reset();

	// Ÿ�̸Ӱ� ��ǥ �ð��� ����ߴ��� Ȯ��
	inline bool IsTimeOut() const { return elapsedTime >= targetTime; }

	// ��ǥ �ð��� ������ �� ����� Setter
	inline void SetTime(float targetTime) { this->targetTime = targetTime; }

private:
	// ��� �ð�, ��ǥ �ð�
	float elapsedTime = 0;
	float targetTime = 0;
};