#pragma once

#include <GL/glew.h>
#include "Console.h"
#define ASSERT(x) if (!(x)) __debugbreak();
#define unlikely(x) x
#define ERR_FAIL_COND_MSG(m_cond, m_msg)                                                                                                \
	if (unlikely(m_cond)) {                                                                                                                         \
		Console::Log(__FUNCTION__, LogMode::ERROR __FILE__, __LINE__, "Condition \"" #m_cond "\" is true. Returning: ",  m_msg); \                                                                                                                   

#ifndef GL_DEBUG
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#else
#define GLCall(x) x
#endif
void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);
