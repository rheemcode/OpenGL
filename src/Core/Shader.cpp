#include "Shader.h"	
#include <GLEW/include/GL/glew.h>
#include <iostream>
#include <fstream>
#include "Renderer.h"

Shader::Shader(const std::string& filePath)
{
	ParseShader(filePath);
}

Shader::Shader()
{

}

void Shader::ParseShader(const std::string& filePath)
{

	std::ifstream stream(filePath);

	std::string line;
	std::stringstream ss[2];

	const char* shaderVar = "#shader";

	while (getline(stream, line))
	{
		if (line.find(shaderVar) != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				shaderType = Type::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				shaderType = Type::FRAGMENT;
				//
		}

		else
		{
			ss[(int)shaderType] << line << '\n';
		}
	}

	vertexSource = ss[0].str();
	fragmentSource = ss[1].str();
	CreateShader();
}

void Shader::CreateShader()
{
    program = glCreateProgram();
	unsigned int vs = CompileShader(vertexSource, GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));
}

unsigned int Shader::CompileShader(const std::string& src, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* s = src.c_str();
	glShaderSource(id, 1, &s, nullptr);
	glCompileShader(id);

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		std::ofstream file("log_output", std::ios_base::app);
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)_malloca(length * sizeof(char));
		//file.write("\n", 1);
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		file << message;
		file.flush();
		file.close();
		std::cout << "FAILED TO COMPILE SHADER\n";
		std::cout << message << "\n";
		return 0;
	}
	return id;
}


