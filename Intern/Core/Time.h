#pragma once
#include <Windows.h>
#include <cstdint>
#include <malloc.h>

struct Time
{
private:
	static Time* s_Time; 

public:
	static uint64_t frequency;
	static uint64_t offset;
	

	static void Init();
	static float GetTime();
	static Time* GetSingleton() { return s_Time; }
};