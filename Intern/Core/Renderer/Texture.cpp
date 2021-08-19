#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_WINDOWS_UTF8

#include <stb_image.h>
#include <Renderer/Debug.h>
#include <array>

#define TEXTURE_INIT_2D GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, m_InternalFormat, m_Width, m_Height)); \
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)); \
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)); \
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); \
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

#define TEXTURE_PARAM_2D GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)); \
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)); \
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); \
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

#define TEXTURE_INIT_CUBE_MAP GLCall(glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, 256, 256)); \


#define TEXTURE_PARAM_CUBE_MAP GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); \
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)); \
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); \
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

void Texture::Bind()
{
	GLCall(glActiveTexture(GL_TEXTURE1));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID[0]));
}

void Texture::ActiveTexture(uint32_t count)
{
	if (textureCount > 1)
	{
		// GL_TEXTURE because GL_TEXTURE0 is shadowMap texture
		glActiveTexture(GL_TEXTURE0 + count);
		return;
	}

	GLCall(glActiveTexture(GL_TEXTURE1));
}

void Texture::Bind(uint32_t p_val)
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID[p_val - 1]));
}

void Texture::BindAll()
{
	for (int i = 0; i < textureCount; i++)
	{
		GLCall(glActiveTexture(GL_TEXTURE1 + i));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_ID[i]));
	}
}

void Texture::BindCubeMap()
{
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID[0]));
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(10, m_ID);
}



void Texture::BufferData(unsigned char* data, int width, int height, DataFormat format)
{

	m_Width = width;
	m_Height = height;
	switch (format)
	{
	case Texture::RGB:
		m_Components = 3;
		break;
	case Texture::RGBA:
		m_Components = 4;
		break;
	default:
		break;
	}
	if (data)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data));
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

uint32_t Texture::AddImage(const std::string& filepath)
{

	const unsigned char* imgData = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_Components, 0);

	if (m_Components == 3)
	{
		m_DataFormat = RGB;
		m_InternalFormat = RGB8;
	}

	if (m_Components == 4)
	{
		m_DataFormat = RGBA;
		m_InternalFormat = RGBA8;
	}

	if (imgData)
	{
		++textureCount;
		ActiveTexture(textureCount);
		Bind(textureCount);
		stbi_set_flip_vertically_on_load(1);

		TEXTURE_PARAM_2D
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, imgData));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		levels += 1;
	}

	xOffset += (m_Width);
	yOffset += (m_Height);
	stbi_image_free((void*)imgData);
	return textureCount;
}

uint32_t Texture::AddImage(const std::string& filepath, uint32_t id)
{
	ActiveTexture(id);
	Bind(id);
	const unsigned char* imgData = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_Components, 0);

	if (m_Components == 3)
	{
		m_DataFormat = RGB;
		m_InternalFormat = RGB8;
	}

	if (m_Components == 4)
	{
		m_DataFormat = RGBA;
		m_InternalFormat = RGBA8;
	}
	if (imgData)
	{
		glTexSubImage2D(GL_TEXTURE_2D, 1, xOffset, yOffset, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, imgData);
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}

	xOffset += m_Width;
	yOffset += m_Height;
	stbi_image_free((void*)imgData);
	return textureCount++;
}


void Texture::AddCubeMapImage(const std::array<std::string, 6>& p_files)
{
	glActiveTexture(GL_TEXTURE0);
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID[0]));

	//stbi_set_flip_vertically_on_load(1);
	TEXTURE_INIT_CUBE_MAP
		//stbi_convert_wchar_to_utf8

		for (int i = 0; i < 6; i++)
		{
			auto& imgFile = p_files[i];
			const uint8_t* imgData = stbi_load(imgFile.c_str(), &m_Width, &m_Height, &m_Components, 0);

			if (m_Components == 3)
			{
				m_DataFormat = RGB;
				m_InternalFormat = RGB8;
			}

			if (m_Components == 4)
			{
				m_DataFormat = RGBA;
				m_InternalFormat = RGBA8;
			}



			int target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
			GLCall(glTexSubImage2D(target, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, imgData));
		}

	TEXTURE_PARAM_CUBE_MAP
}

Texture::Texture(uint32_t count)
	: m_Width(1920), m_Height(1080), m_Components(4), m_DataFormat(RGBA), m_InternalFormat(RGBA8), xOffset(0), yOffset(0)
{
	glGenTextures(count, m_ID);
}

Texture::Texture(uint32_t width, uint32_t height)
	: m_Width(width), m_Height(height), m_Components(4), m_DataFormat(RGB), m_InternalFormat(RGB8), xOffset(0), yOffset(0)
{
	glGenTextures(1, m_ID);
	ActiveTexture(0);
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID[0]));

	TEXTURE_INIT_2D
}


Texture::Texture(const std::string& filename, uint32_t count)
	: xOffset(0), yOffset(0)
{
	stbi_set_flip_vertically_on_load(1);
	const unsigned char* imgData = stbi_load(filename.c_str(), &m_Width, &m_Height, &m_Components, 0);
	if (!imgData)
	{
		stbi_image_free((void*)imgData);
		return;
	}

	if (m_Components == 3)
	{
		m_DataFormat = RGB;
		m_InternalFormat = RGB8;
	}

	if (m_Components == 4)
	{
		m_DataFormat = RGBA;
		m_InternalFormat = RGBA8;
	}

	glGenTextures(count, m_ID);
	Bind();
	TEXTURE_INIT_2D
		GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, imgData));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
}

Texture::~Texture()
{
	Delete();
}