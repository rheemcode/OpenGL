#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Math/Vector2.h"
#include <Math/Matrix4x4.h>
#include <unordered_map>

struct ShaderCache
{
	std::unordered_map<std::string, int> uniformNamesLocation;

	int GetUniformLocation(const std::string& name) const
	{
		auto result = uniformNamesLocation.find(name);
		if (result != uniformNamesLocation.end())
		{
			return result->second;
		}
		return -1;
	}

	void AddUniformNameLocation(const std::string name, int location)
	{
		uniformNamesLocation[name] = location;
	}
};

struct Shader
{
	ShaderCache cache;
	std::vector<std::string> uniformNames;

	enum class Type
	{
		NONE = -1,
		VERTEX,
		FRAGMENT
	};

	Type shaderType = Type::NONE;
	unsigned int program;
	unsigned int CompileShader(const std::string& src, unsigned int type);
	void Bind() const;
	void CreateShader();

	void UploadUniformMat4(const std::string& name, const Matrix4x4& p_mat4) const ;
	void UploadUniformVec4(const std::string& name, const SimpleVec4& p_vec4) const ;
	void UploadUniformVec3(const std::string& name, const Vector3& p_vec3) const;
	void UploadUniformVec2(const std::string& name, const Vector2& p_vec2) const;
	void UploadUniformInt(const std::string& name, int p_val) const;
	void UploadUniformFloat(const std::string& name, float p_val) const;
	void UploadUniformIntArray(const std::string& name, int* p_val, uint32_t count) const;

public:
	Shader();
	Shader(const std::string& filePath);
	void SetMat4(const std::string& name, const Matrix4x4& p_mat4);
	void SetVec4(const std::string& name, const SimpleVec4& p_vec4);
	void SetVec3(const std::string& name, const Vector3& p_vec3);
	void SetVec2(const std::string& name, const Vector2& p_vec2);
	void SetInt(const std::string& name, int p_val);
	void SetFloat(const std::string& name, float p_val);
	void SetIntArray(const std::string& name, int* p_val, uint32_t count);
	
	std::string vertexSource, fragmentSource;
	void ParseShader(const std::string& filepath);
	unsigned int GetProgram() const { return program; };

};

