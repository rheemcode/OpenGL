#pragma once
#include "GL/glew.h"
#include <cstdint>

enum ShaderType
{
	SHADER_VERTEX = GL_VERTEX_SHADER,
	SHADER_GEOMETRY = GL_GEOMETRY_SHADER,
	SHADER_FRAGMENT = GL_FRAGMENT_SHADER,
	SHADER_COMPUTE = GL_COMPUTE_SHADER,
	MAX = 4
};


class Shader;

struct ShaderCompiler
{
	struct ShaderMap
	{
		std::stringstream shaderSource;
		ShaderType type = ShaderType::MAX;
	};
	Shader* shader;
	void ParseShader(Shader* shader);
	void LinkShader();
	int CheckLinkErrors(uint32_t program);
	int CheckCompileError(ShaderType shaderType, uint32_t shader);
	void AttachShaderToProgram(uint32_t program, uint32_t shader);
	void CompileShader(const std::string& shaderSource, ShaderType shaderType);
};
