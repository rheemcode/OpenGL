#include "Shader.h"	
#include <GL/glew.h>
#include <iostream>
#include <sstream>
#include "Debug.h"
#include "Math/Vector2.h"
#include "Core/Console.h"

Shader::Shader(const std::string& filePath)
{
	ParseShader(filePath);
}

void Shader::ParseShader(const std::string& filePath)
{

	std::ifstream stream(filePath);

	std::string line;
	std::stringstream ss[2];

	const char* shaderVar = "#shader";
	if (!stream)
		return;

	while (getline(stream, line))
	{

		if (line.find(shaderVar) != std::string::npos && line.find("//", 0, 2) == std::string::npos)
		{

			if (line.find("vertex") != std::string::npos)
				shaderType = Type::VERTEX;

			else if (line.find("fragment") != std::string::npos)
				shaderType = Type::FRAGMENT;
		}

		else
		{
			ss[(int)shaderType] << line << '\n';
		}

		if (line.find("uniform") != std::string::npos)
		{
			const char* str = line.c_str();
			std::string result = "";
			while ((str = std::strstr(str, " ")) != NULL)
			{
				++str;
				result = str;
				result = result.substr(0, result.size() - 1);
			}
			
			size_t pos;
			if ((pos = result.find('[')) != result.npos)
			{
				const std::string numstr = result.substr(pos + 1, (result.size() - pos) - 2);
				int to = numstr.length();
				int64_t integer = 0;
				int64_t sign = 1;

				for (int i = 0; i < to; i++)
				{
					char32_t c = numstr[i];
					if (c >= '0' && c <= '9')
					{
						integer *= 10;
						integer += c - '0';
					}
				}

				for (int i = 0; i < integer; i++)
				{
					std::stringstream _ss;
					_ss << result.substr(0, pos) << "[" << i << "]";
					uniformNames.push_back(_ss.str());
				}
			}
			else
			{
				uniformNames.push_back(result);
			}
		}

	}

	vertexSource = ss[0].str();
	fragmentSource = ss[1].str();
	CreateShader();
}

void Shader::CreateShader()
{
    program = glCreateProgram();
	uint32_t vs;
	uint32_t fs;

	if (vertexSource.size() != 0)
	{
		vs = CompileShader(vertexSource, GL_VERTEX_SHADER);
		GLCall(glAttachShader(program, vs));
	}

	if (fragmentSource.size() != 0)
	{
		fs = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);
		GLCall(glAttachShader(program, fs));
	}
	

	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	if (vertexSource.size() != 0)
		glDeleteShader(vs);
	if (fragmentSource.size() != 0)
		glDeleteShader(fs);

	glUseProgram(program);
	
	for (const auto& name : uniformNames)
	{
		cache.AddUniformNameLocation(name, glGetUniformLocation(program, name.c_str()));
	}

	uniformNames.clear();
}

unsigned int Shader::CompileShader(const std::string& src, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* s = src.c_str();
	glShaderSource(id, 1, &s, nullptr);
	glCompileShader(id);

	int result = 0;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		std::stringstream ss;
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)_malloca(length * sizeof(char));
		//file.write("\n", 1);
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		ss << message;
		Console::Log(ss.str().c_str(), LogMode::ERROR);
		return 0;
	}
	return id;
}

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