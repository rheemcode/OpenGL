#pragma once
#include <string>
#include <Windows.h>
#include <cstdint>
#undef ERROR

enum LogMode
{
	DEBUG,
	SUCCESS,
	ERROR
};


class Console
{
	static Console* s_Instance;
	void Init();

	static void Log(const std::string& p_msg, LogMode p_logMode, const std::string& p_msg2);
	static void Log(const std::string& p_msg, LogMode p_logMode, const std::string& p_msg2, const std::string& p_msg3);
	static void Log(const std::string& p_msg, LogMode p_logMode, const std::string& p_msg2, const std::string& p_msg3, const std::string& p_msg4);
	static void Log(const std::string& p_msg, LogMode p_logMode, const std::string& p_msg2, const std::string& p_msg3, const std::string& p_msg4, const std::string& p_msg5);
	static void Error(const std::string& p_msg);
public:
	template<typename... Args>
	static void Log(const std::string& p_msg, const Args&... args);
	static void Log(const std::string& p_msg, LogMode p_logMode = LogMode::DEBUG);
	static void Create();

};
