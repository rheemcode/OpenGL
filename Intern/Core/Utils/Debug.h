#pragma once

#include "Console.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GL_CRASH __debugbreak();

#define unlikely(x) x
#define likely(x) !x

#define ERR_CRASH_COND(m_cond)\
	if (unlikely(x)) { \
		Console::Log( LogMode::ERROR,__FUNCTION__, __FILE__, std::to_string(__LINE__), "Condition \"" #m_cond "\" is true.")\
		GL_CRASH \
}

#define ERR_FAIL_COND_MSG(m_cond, m_msg)                                                                                         \
	if (unlikely(m_cond)) {                                                                                                      \
		Console::Log( LogMode::ERROR, __FUNCTION__, __FILE__, std::to_string(__LINE__), "Condition \"" #m_cond "\" is true. ",  m_msg); \
		return; \
}

#define ERR_FAIL_COND_V_MSG(m_cond, m_msg, val)                                                                                     \
	if (unlikely(m_cond)) {                                                                                                         \
		Console::Log( LogMode::ERROR, __FUNCTION__, __FILE__, std::to_string(__LINE__), "Condition \"" #m_cond "\" is true. Returning: " #val,  m_msg);    \
		return val;																													\
}

#define ERR_CRASH_COND_V_MSG(m_cond, m_msg, val)                                                                                     \
	if (unlikely(m_cond)) {                                                                                                         \
		Console::Log( LogMode::ERROR, __FUNCTION__, __FILE__, std::to_string(__LINE__), "Condition \"" #m_cond "\" is true. Returning: " #val,  m_msg);    \
		GL_CRASH\
		return val;																													\
}
#define GL_DEBUG
#ifdef GL_DEBUG

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));

#else

#define GLCall(x) x

#endif

void GLIB_API GLClearError();
bool GLIB_API GLLogCall(const char* function, const char* file, int line);
