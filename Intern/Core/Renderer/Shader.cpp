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

Shader::Shader()
{

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

	glDeleteShader(vs);
	glDeleteShader(fs);
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
		Console::Error(ss.str().c_str());
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
	Bind();
	GLint location = glGetUniformLocation(program, name.c_str());
	GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &p_mat4[0].x));
}

void Shader::UploadUniformVec4(const std::string& name, const SimpleVec4& p_vec4) const
{
	GLint location = glGetUniformLocation(program, name.c_str());
	GLCall(glUniform4f(location, p_vec4.x, p_vec4.y, p_vec4.z, p_vec4.w));
}

void Shader::UploadUniformVec3(const std::string& name, const Vector3& p_vec3) const
{
	GLint location = glGetUniformLocation(program, name.c_str());
	GLCall(glUniform3f(location, p_vec3.x, p_vec3.y, p_vec3.z));
}

void Shader::UploadUniformVec2(const std::string& name, const Vector2& p_vec2) const
{
	GLint location = glGetUniformLocation(program, name.c_str());
	GLCall(glUniform2f(location, p_vec2.x, p_vec2.y));
}

void Shader::UploadUniformInt(const std::string& name, int p_val) const
{
	GLint location = glGetUniformLocation(program, name.c_str());
	GLCall(glUniform1i(location, p_val));
}

void Shader::UploadUniformFloat(const std::string& name, float p_val) const
{
	GLint location = glGetUniformLocation(program, name.c_str());
	GLCall(glUniform1f(location, p_val));
}

void Shader::UploadUniformIntArray(const std::string& name, int* p_val, uint32_t count) const
{
	GLint location = glGetUniformLocation(program, name.c_str());
	glUniform1iv(location, count, p_val);
}