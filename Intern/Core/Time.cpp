#include "Time.h"

#include <wctype.h>
#include <windows.h>

uint64_t Time::frequency = 0;
uint64_t Time::offset = 0;


float Time::GetTime()
{
	uint64_t value;
	//QueryPerformanceCounter((LARGE_INTEGER*)&value);
//	return (float)(value - offset) / frequency;
}

void Time::Init()
{
	
	if (!s_Time)
	{
		s_Time = (Time*)_malloca(sizeof(uint32_t) * 2);
		//QueryPerformanceCounter((LARGE_INTEGER*)&offset);
		//QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	}
	
}