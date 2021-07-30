#pragma once
#include <Windows.h>
#include <cstdint>

struct Time
{
	uint64_t ticksStart;
	uint64_t frequency;
	static Time* s_Instance;

	void Init();

	uint64_t GetTicks();

	float GetTicks2();

	static void Create();
	static Time* GetSingleton() { return s_Instance; }
};

