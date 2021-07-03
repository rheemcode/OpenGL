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
        //   fs << glewGetString(error) << std::endl;
        fs << "[OPEN GL Error] {" << error << "}" << function << file << line << std::endl;
        //   fs << glewGetString(error) << std::endl;

        fs.flush();
        fs.close();
        return false;
    }

    return true;
}