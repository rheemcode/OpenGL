#include <glpch.h>
#include <GL/glew.h>
#include "Shader.h"	
#include "ShaderCompiler.h"
#include "Debug.h"
#include "Math/Vector2.h"
#include "Utils/Console.h"

void Shader::Bind() const
{
	GLCall(glUseProgram(program));
}

void Shader::SetMat4(const std::string& name, const Matrix4x4& p_mat4)
{
	UploadUniformMat4(name, p_mat4);
}

void Shader::SetVec4(const std::string& name, const SimpleVec4& p_vec4)
{
	UploadUniformVec4(name, p_vec4);
}

void Shader::SetVec3(const std::string& name, const Vector3& p_vec3)
{
	UploadUniformVec3(name, p_vec3);
}

void Shader::SetVec2(const std::string& name, const Vector2& p_vec2)
{
	UploadUniformVec2(name, p_vec2);
}
void Shader::SetInt(const std::string& name, int p_val)
{
	UploadUniformInt(name, p_val);
}
void Shader::SetFloat(const std::string& name, float p_val)
{
	UploadUniformFloat(name, p_val);
}
void Shader::SetIntArray(const std::string& name, int* p_val, uint32_t count)
{
	UploadUniformIntArray(name, p_val, count);
}

void Shader::UploadUniformMat4(const std::string& name, const Matrix4x4& p_mat4) const
{
	GLint location = cache.GetUniformLocation(name);
	if (location == -1)
		return;
	GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &p_mat4[0].x));
}

void Shader::UploadUniformVec4(const std::string& name, const SimpleVec4& p_vec4) const
{
	GLint location = cache.GetUniformLocation(name);
	if (location == -1)
		return;
	GLCall(glUniform4f(location, p_vec4.x, p_vec4.y, p_vec4.z, p_vec4.w));
}

void Shader::UploadUniformVec3(const std::string& name, const Vector3& p_vec3) const
{
	GLint location = cache.GetUniformLocation(name);
	if (location == -1)
		return;
	GLCall(glUniform3f(location, p_vec3.x, p_vec3.y, p_vec3.z));
}

void Shader::UploadUniformVec2(const std::string& name, const Vector2& p_vec2) const
{
	GLint location = cache.GetUniformLocation(name);
	if (location == -1)
		return;
	GLCall(glUniform2f(location, p_vec2.x, p_vec2.y));
}

void Shader::UploadUniformInt(const std::string& name, int p_val) const
{
	GLint location = cache.GetUniformLocation(name);
	if (location == -1)
		return;
	GLCall(glUniform1i(location, p_val));
}

void Shader::UploadUniformFloat(const std::string& name, float p_val) const
{
	GLint location = cache.GetUniformLocation(name);
	if (location == -1)
		return;
	GLCall(glUniform1f(location, p_val));
}

void Shader::UploadUniformIntArray(const std::string& name, int* p_val, uint32_t count) const
{
	GLint location = cache.GetUniformLocation(name);
	if (location == -1)
		return;
	glUniform1iv(location, count, p_val);
}

Shader::Shader()
{
	program = glCreateProgram();
}

Shader::Shader(const std::string& p_filePath)
	: filePath(p_filePath)
{
	program = glCreateProgram();
	ShaderCompiler compiler;

	// should be the other way round
	compiler.ParseShader(this);

	Bind();
	for (const auto& name : uniformNames)
	{
		cache.AddUniformNameLocation(name, glGetUniformLocation(program, name.c_str()));
	}

	uniformNames.clear();
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

void ComputeShader::Dispatch(uint32_t x, uint32_t y, uint32_t z)
{
	glDispatchCompute(x, y, z);
}

ComputeShader::ComputeShader()
	: Shader() {}

ComputeShader::ComputeShader(const std::string& p_filePath)
	: Shader(p_filePath) {}