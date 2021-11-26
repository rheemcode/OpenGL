#include <glpch.h>

#include "Debug.h"
#include <GL/glew.h>


void GLClearError()
{
    while (glGetError());
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::stringstream ss;
   
        std::string errorName;

        switch (error)
        {
        case GL_INVALID_ENUM:
            errorName = "Invalid Enum";
            break;
        case GL_INVALID_VALUE:
            errorName = "Invalid Value";
            break;
        case GL_INVALID_OPERATION:
            errorName = "Invalid Operation";
            break;
        case GL_STACK_OVERFLOW:
            errorName = "Stack Overflow";
            break;
        case GL_STACK_UNDERFLOW:
            errorName = "Stack Underflow";
            break;
        case GL_OUT_OF_MEMORY:
            errorName = "Out Of Memory";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            errorName = "Invalid FrameBuffer Operation";
            break;
        }
        ss << "[OPEN GL Error] {" << error << "}" << "[" << errorName << "]" <<
            "\nFile: "     << file     << 
            "\nFunction: " << function << 
            "\nLine: "     << line     << "\n";

        Console::Log(LogMode::ERROR, ss.str().c_str());
        return false;
    }

    return true;
}