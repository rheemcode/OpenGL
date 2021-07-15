#include "Console.h"

Console* Console::s_Instance;

void Console::Create()
{
	if (!s_Instance)
	{
		s_Instance = new Console();
		s_Instance->Init();
	}
}


void Console::Init()
{
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	AllocConsole();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);

	coninfo.dwSize.Y = 500;

	WORD current_bg = coninfo.wAttributes & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);

	uint32_t basecol;

	basecol = FOREGROUND_GREEN;
	basecol |= current_bg;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), basecol | FOREGROUND_INTENSITY);
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
	SetConsoleTitle(L"OpenGL Debug");
}


void Console::Log(const std::string& p_msg)
{
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);

	WORD current_bg = coninfo.wAttributes & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	uint32_t basecol;
	basecol = FOREGROUND_GREEN;
	basecol |= current_bg;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), basecol | FOREGROUND_INTENSITY);
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), p_msg.c_str(), p_msg.size(), nullptr, NULL);
}


void Console::Error(const std::string& p_msg)
{
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);

	WORD current_bg = coninfo.wAttributes & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	uint32_t basecol;
	basecol = FOREGROUND_RED;
	basecol |= current_bg;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), basecol | FOREGROUND_INTENSITY);
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), p_msg.c_str(), p_msg.size(), nullptr, NULL);
}