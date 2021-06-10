#pragma once
#include <fstream>
#include <string>
#include <sstream>

struct Shader
{

public:
	Shader();
	Shader(const std::string& filePath);
	std::string vertexSource, fragmentSource;

	void ParseShader(const std::string& filepath);

	unsigned int GetProgram() const { return program; };

private:
	unsigned int program;
	unsigned int CompileShader(const std::string& src, unsigned int type);
	void CreateShader();

private:
	enum class Type
	{
		NONE = -1,
		VERTEX,
		FRAGMENT
	};

	Type shaderType = Type::NONE;
};

