#include "Timestep.h"

#include <malloc.h>


::Time* Time::s_Instance = nullptr;



uint64_t Time::GetTicks()
{
	uint64_t ticks;

	// This is the number of clock ticks since start
	if (!QueryPerformanceCounter((LARGE_INTEGER*)&ticks))
		//	ticks = (UINT64)timeGetTime();
		int i = 1;

	// Divide by frequency to get the time in seconds
	// original calculation shown below is subject to overflow
	// with high ticks_per_second and a number of days since the last reboot.
	// time = ticks * 1000000L / ticks_per_second;

	// we can prevent this by either using 128 bit math
	// or separating into a calculation for seconds, and the fraction
	uint64_t seconds = ticks / frequency;

	// compiler will optimize these two into one divide
	uint64_t leftover = ticks % frequency;

	// remainder
	uint64_t time = (leftover * 1000000L) / frequency;

	// seconds
	time += seconds * 1000000L;

	// Subtract the time at game start to get
	// the time since the game started
	time -= ticksStart;
	return time;
}

float Time::GetTicks2()
{
	uint64_t ticks;


	QueryPerformanceCounter((LARGE_INTEGER*)&ticks);
	float val = (float) (ticks - ticksStart) / frequency;
	return val;
}

void Time::Create()
{

	if (!s_Instance)
	{
		s_Instance = (Time*)_malloca(sizeof(uint64_t) * 2);
		s_Instance->Init();
	}
}


void Time::Init()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&ticksStart);
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
}
