#pragma once
#include <string>
#include <Windows.h>
#include <cstdint>

class Console
{
	static Console* s_Instance;
	void Init();

public:
	static void Log(const std::string& p_msg);
	static void Error(const std::string& p_msg);
	static void Create();

};


