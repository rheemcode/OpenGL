#pragma once
#include "GLCore.h"

#undef ERROR

enum class LogMode
{
	DEBUG,
	SUCCESS,
	ERROR
};


class GLIB_API Console
{
	static Console* s_Instance;
	void Init();

	static void Error(const std::string& p_msg);
public:

	static void Destroy() { delete s_Instance; }
	static void Log(LogMode p_logMode, const std::string& p_msg);
	static void Log(LogMode p_logMode, const std::string& p_msg, const std::string& p_msg2);
	static void Log(LogMode p_logMode, const std::string& p_msg, const std::string& p_msg2, const std::string& p_msg3);
	static void Log(LogMode p_logMode, const std::string& p_msg, const std::string& p_msg2, const std::string& p_msg3, const std::string& p_msg4);
	static void Log(LogMode p_logMode, const std::string& p_msg, const std::string& p_msg2, const std::string& p_msg3, const std::string& p_msg4, const std::string& p_msg5);
	static void Log(LogMode p_logMode, const char* p_msg);
	static void Log(LogMode p_logMode, const char* p_msg, const char* p_msg2);
	template<class... Args>
	static void Log(LogMode p_logMode, Args&... args);
	static void Create();
};
