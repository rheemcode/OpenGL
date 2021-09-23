#include "Texture.h"
#include <glpch.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_WINDOWS_UTF8

#include <stb_image.h>
#include <Debug.h>


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


Texture* Texture::s_defaultTexure = nullptr;
uint32_t Texture::s_defaultTexID;

void Texture::CreateDefaultTexture()
{
	if (s_defaultTexure == nullptr)
	{
		s_defaultTexure = new Texture();
		float (*textureData)[3] = new float[4 * 4][3];
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				textureData[x + y * 4][0] = 255.f;
				textureData[x + y * 4][1] = 255.f;
				textureData[x + y * 4][2] = 255.f;

			}
		}

		glGenTextures(1, &s_defaultTexID);
		glBindTexture(GL_TEXTURE_2D, s_defaultTexID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 4, 4, 0, GL_RGB, GL_FLOAT, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
		delete[] textureData;
	}
}

void Texture::Bind()
{
	//GLCall(glActiveTexture(GL_TEXTURE1));
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
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID[p_val]));
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
	glDeleteTextures(1, &s_defaultTexID);
	glDeleteTextures(32, m_ID);
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

void Texture::Screenshot()
{
	//glReadBuffer(GL_BACK);
	//void* imgData = (void*) malloc(1200 * 700);
	//GLCall(glReadPixels(0, 0, 1200, 700, GL_DEPTH, GL_UNSIGNED_BYTE, imgData));
}

uint32_t Texture::AddImage(const std::string& filepath)
{
	stbi_set_flip_vertically_on_load(1);
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
	//	ActiveTexture(textureCount);
		Bind(0);


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
	GLCall(glGenTextures(count, m_ID));
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