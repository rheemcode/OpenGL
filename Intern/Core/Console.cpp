#include <glpch.h>
#include "Console.h"

#define SET_CONSOLE_ATTRIB_DEBUG\
	CONSOLE_SCREEN_BUFFER_INFO coninfo; \
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);\
	WORD current_bg = coninfo.wAttributes & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY); \
	uint32_t basecol; \
	basecol = FOREGROUND_INTENSITY; \
	basecol |= current_bg; \
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), basecol | FOREGROUND_INTENSITY);

#define SET_CONSOLE_ATTRIB_SUCCESS \
	CONSOLE_SCREEN_BUFFER_INFO coninfo; \
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);\
	WORD current_bg = coninfo.wAttributes & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY); \
	uint32_t basecol; \
	basecol = FOREGROUND_GREEN; \
	basecol |= current_bg; \
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), basecol | FOREGROUND_INTENSITY);

#define SET_CONSOLE_ATTRIB_ERROR \
	CONSOLE_SCREEN_BUFFER_INFO coninfo; \
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);\
	WORD current_bg = coninfo.wAttributes & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY); \
	uint32_t basecol; \
	basecol = FOREGROUND_RED; \
	basecol |= current_bg; \
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), basecol | FOREGROUND_INTENSITY);

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


void Console::Log(const std::string& p_msg, LogMode p_logMode)
{
	if (p_logMode == LogMode::DEBUG)
	{
		SET_CONSOLE_ATTRIB_DEBUG
	}
	else if (p_logMode == LogMode::SUCCESS)
	{
		SET_CONSOLE_ATTRIB_SUCCESS
	}
	else
	{
		SET_CONSOLE_ATTRIB_ERROR
	}

	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), p_msg.c_str(), p_msg.size(), nullptr, NULL);
}


void Console::Log(const std::string& p_msg, LogMode p_logMode, const std::string& p_msg2)
{
	if (p_logMode == LogMode::DEBUG)
	{
		SET_CONSOLE_ATTRIB_DEBUG
	}
	else if (p_logMode == LogMode::SUCCESS)
	{
		SET_CONSOLE_ATTRIB_SUCCESS
	}
	else
	{
		SET_CONSOLE_ATTRIB_ERROR
	}

	std::stringstream ss;
	ss << " " << p_msg << " " << p_msg2 << "\n";
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), ss.str().c_str(), p_msg.size(), nullptr, NULL);
}

void Console::Log(const std::string& p_msg, LogMode p_logMode, const std::string& p_msg2, const std::string& p_msg3)
{
	if (p_logMode == LogMode::DEBUG)
	{
		SET_CONSOLE_ATTRIB_DEBUG
	}
	else if (p_logMode == LogMode::SUCCESS)
	{
		SET_CONSOLE_ATTRIB_SUCCESS
	}
	else
	{
		SET_CONSOLE_ATTRIB_ERROR
	}


	std::stringstream ss;
	ss << " " << p_msg << " " << p_msg2 << " " << p_msg3 << "\n";
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), ss.str().c_str(), p_msg.size(), nullptr, NULL);
}

void Console::Log(const std::string& p_msg, LogMode p_logMode, const std::string& p_msg2, const std::string& p_msg3, const std::string& p_msg4)
{
	if (p_logMode == LogMode::DEBUG)
	{
		SET_CONSOLE_ATTRIB_DEBUG
	}
	else if (p_logMode == LogMode::SUCCESS)
	{
		SET_CONSOLE_ATTRIB_SUCCESS
	}
	else
	{
		SET_CONSOLE_ATTRIB_ERROR
	}


	std::stringstream ss;
	ss << " " << p_msg << " " << p_msg2 << " " << p_msg3 << " " << p_msg4 << "\n";
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), ss.str().c_str(), p_msg.size(), nullptr, NULL);
}

void Console::Log(const std::string& p_msg, LogMode p_logMode, const std::string& p_msg2, const std::string& p_msg3, const std::string& p_msg4, const std::string& p_msg5)
{
	if (p_logMode == LogMode::DEBUG)
	{
		SET_CONSOLE_ATTRIB_DEBUG
	}
	else if (p_logMode == LogMode::SUCCESS)
	{
		SET_CONSOLE_ATTRIB_SUCCESS
	}
	else
	{
		SET_CONSOLE_ATTRIB_ERROR
	}

	std::stringstream ss;
	ss << " " << p_msg << " " << p_msg2 << " " << p_msg3 << " " << p_msg4 << " " << p_msg5 << "\n";
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), ss.str().c_str(), p_msg.size(), nullptr, NULL);
}


template<typename... Args>
void Console::Log(const std::string& p_msg, const Args&... args)
{
	Console::Log(p_msg,  args...);
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