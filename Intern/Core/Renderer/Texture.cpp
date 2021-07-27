#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <Renderer/Debug.h>

uint32_t Texture::texCount = 0;
uint32_t Texture::texturesIDs[32];

void Texture::Bind()
{

	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID[0]));
}

void Texture::Bind(uint32_t p_val)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + p_val));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID[p_val]));
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	// TODO: what the heck
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
	Bind(texCount);

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


	GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, m_InternalFormat, m_Width, m_Height));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	if (imgData)
	{
		GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, imgData));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		levels += 1;
	}

	xOffset += (m_Width);
	yOffset += (m_Height);
	stbi_image_free((void*)imgData);
	return texCount++;
}

uint32_t Texture::AddImage(const std::string& filepath, uint32_t id)
{
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
	return texCount++;
}


Texture::Texture(uint32_t count)
	: m_Width(1920), m_Height(1080), m_Components(4), m_DataFormat(RGBA), m_InternalFormat(RGBA8), xOffset(0), yOffset(0)
{
	//GLCall(glActiveTexture(GL_TEXTURE0 + texCount));
	glGenTextures(count, m_ID);
}

Texture::Texture(uint32_t width, uint32_t height)
	: m_Width(width), m_Height(height), m_Components(4), m_DataFormat(RGB), m_InternalFormat(RGB8), xOffset(0), yOffset(0)
{


	//GLCall(glActiveTexture(GL_TEXTURE0 + texCount));
	glGenTextures(1, m_ID);

	GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, m_InternalFormat, m_Width, m_Height));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID[0]));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
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
	GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, m_InternalFormat, m_Width, m_Height));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, imgData));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
}

Texture::~Texture()
{
	Delete();
}
