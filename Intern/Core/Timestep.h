#pragma once
#include "GLCore.h"

struct GLIB_API Time
{
	uint64_t ticksStart;
	uint64_t frequency;
	float elapsedTime;
	static Time* s_Instance;

	void Init();

	uint64_t GetTicks();
	float GetElapsedTime();
	static void Create();
	static Time* GetSingleton() { return s_Instance; }
};

