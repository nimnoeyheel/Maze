#pragma once

// �̸� �����ϵ� ���
#include <Windows.h>
#include <iostream>

//#define _CRPDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//
//// �޸� ���� �Լ�
//template<typename T>
//void SafeDelete(T* pointer)
//{
//	if (pointer)
//	{
//		delete pointer;
//		pointer = nullptr;
//	}
//}
//
//// ����� �뵵
//#ifdef _DEBUG
//#define new new ( _NORMAL_BLOCK, __FILE__, __LINE__)
//#else
//#define new new
//#endif
//
//#if ENGINE_BUILD_DLL
//#define ENGINE_API __declspec(dllexport)
//#else
//#define ENGINE_API __declspec(dllexport)
//#endif
