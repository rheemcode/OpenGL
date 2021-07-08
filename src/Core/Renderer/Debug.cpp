#include "Debug.h"
#include <iostream>
#include <fstream>

void GLClearError()
{
    while (glGetError());
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::ofstream fs("log_output", std::ios_base::app);
   
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
        }
        fs << "[OPEN GL Error] {" << error << "}" << "[" << errorName << "]" << function << file << line << std::endl;

        fs.flush();
        fs.close();
        return false;
    }

    return true;
}