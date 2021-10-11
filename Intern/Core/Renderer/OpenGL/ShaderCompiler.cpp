#include <glpch.h>
#include "Debug.h"
#include "Shader.h"
#include "ShaderCompiler.h"

int64_t to_int(const std::string& str)
{
	int to = str.length();
	int64_t integer = 0;
	int64_t sign = 1;

	for (int i = 0; i < to; i++)
	{
		char32_t c = str[i];
		if (c >= '0' && c <= '9')
		{
			integer *= 10;
			integer += c - '0';
		}
	}

	return integer;
}


// Compiling all types of shader in one file
void ShaderCompiler::ParseShader(Shader* p_shader)
{
	shader = p_shader;
	
	const char* shaderVar = new char[8]{ "#shader" };
	const char* vertex = new char[7]{ "vertex" };
	const char* fragment = new char[9]{ "fragment" };
	const char* geometry = new char[9]{ "geometry" };
	const char* compute = new char[8]{ "compute" };

	std::ifstream* stream = new std::ifstream(shader->filePath);
	ERR_FAIL_COND_MSG(!*stream, "Cannot Find Shader File at : " + shader->filePath);

	std::string line;
	std::stringstream ss[static_cast<int>(ShaderType::MAX)];

	ShaderMap* shaderMap = new ShaderMap[ShaderType::MAX];
	
	int index = -1;
	while (getline(*stream, line))
	{
		if (line.find(shaderVar) != std::string::npos && line.find("//", 0, 2) == std::string::npos)
		{
			if (line.find(vertex) != std::string::npos)
			{
				index++;
				shaderMap[index] = { std::stringstream(), ShaderType::SHADER_VERTEX };
			}

			else if (line.find(fragment) != std::string::npos)
			{
				index++;
				shaderMap[index] = { std::stringstream(), ShaderType::SHADER_FRAGMENT };
			}

			else if (line.find(geometry) != std::string::npos)
			{
				index++;
				shaderMap[index] = { std::stringstream(), ShaderType::SHADER_GEOMETRY };
			}
			
			else if (line.find(compute) != std::string::npos)
			{
				index++;
				shaderMap[index] = { std::stringstream(), ShaderType::SHADER_COMPUTE };
			}
		}

		else
		{
			shaderMap[index].shaderSource << line << '\n';
		}

		if (line.find("uniform") != std::string::npos && !line._Starts_with("//"))
		{
			bool isUniformBlock = false;
			std::string uniformBlock;
			int blockBinding = 0;

			if (line._Starts_with("layout"))
			{
				isUniformBlock = true;
				auto start = line.find("binding");

				if (start != line.npos)
				{
					const auto& str = line.substr(line.find("=") + 1);
					blockBinding = to_int(str);
				}
			}

			size_t slash = 0;
			// remove trailing forward slashes at ending
			if ((slash = line.find("//")) != line.npos)
			{
				line = line.substr(0, slash - 1);
			}

			const char* str = line.c_str();
			std::string result = "";

			// extract unifrom name
			while ((str = std::strstr(str, " ")) != NULL)
			{
				++str;
				result = str;
			}

			// remove semicolon at ending
			size_t semicolon;
			size_t braces;

			// predicting semicolon will be at the end 
			// thus all instructions must be on new line
			if ((semicolon = result.find(";")) != result.npos)
			{
				result = result.substr(0, result.size() - 1);
			}

			else if ((braces = result.find("{")) != result.npos)
				result = result.substr(0, result.size() - 1);

			if (isUniformBlock)
			{
				shader->cache.AddUniformBlockBinding(result, blockBinding);
				continue;
			}

			size_t pos;
			if ((pos = result.find('[')) != result.npos)
			{
				shader->uniformNames.push_back(result.substr(0, pos));
				const std::string numstr = result.substr(pos + 1, (result.size() - pos) - 2);
				int num = to_int(numstr);
				for (int i = 0; i < num; i++)
				{
					std::stringstream _ss;
					_ss << result.substr(0, pos) << "[" << i << "]";
					shader->uniformNames.push_back(_ss.str());
				}
			}
			else
			{
				shader->uniformNames.push_back(result);
			}
		}

	}

	for (int i = 0; i < index + 1; i++)
	{
		CompileShader(shaderMap[i].shaderSource.str(), shaderMap[i].type);
	}

	LinkShader();


	delete[] shaderVar;
	delete[] vertex;
	delete[] fragment;
	delete[] geometry;
	delete[] shaderMap;
	delete[] compute;
	delete[] stream;
}

void ShaderCompiler::CompileShader(const std::string& src, ShaderType type)
{
	uint32_t id = glCreateShader(type);
	const auto* s = src.c_str();
	glShaderSource(id, 1, &s, nullptr);
	glCompileShader(id);
	int errorcode = CheckCompileError(type, id);
	if (!errorcode)
	{
		AttachShaderToProgram(shader->program, id);
		glDeleteShader(id);
		return;
	}

	glDeleteShader(id);
}

int ShaderCompiler::CheckLinkErrors(uint32_t program)
{
	std::stringstream ss;
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca((length + 1) * sizeof(char));
		glGetProgramInfoLog(program, length, &length, (message));
		ss << "ERROR::PROGRAM_LINKING_ERROR : " << message << "\n";
		Console::Log(LogMode::ERROR, ss.str().c_str());
		_freea((void*)message);
		return 1;
	}

	return 0;
}

void ShaderCompiler::AttachShaderToProgram(uint32_t program, uint32_t shader)
{
	glAttachShader(program, shader);
}

void ShaderCompiler::LinkShader()
{
	glLinkProgram(shader->program);
	glValidateProgram(shader->program);
	if (CheckLinkErrors(shader->program))
	{
		// or just delete the program
		shader->program = GL_NONE;
	}
}

int ShaderCompiler::CheckCompileError(ShaderType shaderType, uint32_t p_shader)
{
	int result = 0;
	glGetShaderiv(p_shader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		std::stringstream ss;
		ss << "Shader File [ " << shader->filePath << " ] ";
		int length;
		glGetShaderiv(p_shader, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(p_shader, length, &length, message);

		const char* _type = "";
		switch (shaderType)
		{
		case(ShaderType::SHADER_VERTEX): _type = "[ VERTEX SHADER ] ::"; break;
		case(ShaderType::SHADER_FRAGMENT): _type = "[ FRAGMENT SHADER ] ::"; break;
		case(ShaderType::SHADER_GEOMETRY): _type = "[ GEOMETRY SHADER ] ::"; break;
		default: break;
		}

		ss << _type << message << "\n\n";
		Console::Log(LogMode::ERROR, ss.str().c_str());
		return 1;
	}

	return 0;
}


