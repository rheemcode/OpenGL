#include <glpch.h>
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_WINDOWS_UTF8

#include <stb_image.h>
#include "Math/Vector3.h"
#include <Debug.h>

std::shared_ptr<Texture> Texture::s_defaultTexture;
std::shared_ptr<Texture> Texture::s_defaultSpecularTexture;
std::shared_ptr<Texture> Texture::s_defaultNormalTexture;


// TODO: Updating Textures
void Texture::CreateDefaultTextures()
{
	stbi_set_flip_vertically_on_load(true);
	if (s_defaultTexture == nullptr)
	{
		Vector3* texData = new Vector3[4 * 4];

		TextureParameters texParam;
		texParam.dataFormat = TextureFormat::RGB;
		texParam.internalFormat = TextureFormat::RGB8;
		texParam.magFilter = TextureFilter::NEAREST;
		texParam.minFilter = TextureFilter::NEAREST;
		texParam.textureDataType = TextureDataType::FLOAT;
		texParam.wrap = TextureWrap::REPEAT;

		s_defaultTexture = std::make_shared<Texture2D>();
		s_defaultSpecularTexture = std::make_shared<Texture2D>();
		s_defaultNormalTexture = std::make_shared<Texture2D>();

		s_defaultTexture->SetHeight(4);
		s_defaultTexture->SetWidth(4);
		s_defaultNormalTexture->SetHeight(4);
		s_defaultNormalTexture->SetWidth(4);
		s_defaultSpecularTexture->SetHeight(4);
		s_defaultSpecularTexture->SetWidth(4);

		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				texData[x + y * 4] = Vector3(255.f, 255.f, 255.f);
			}
		}

		s_defaultTexture->Create(texData, texParam);

		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				texData[x + y * 4] = Vector3(0.f, 0.f, 0.f);
			}
		}

		s_defaultSpecularTexture->Create(texData, texParam);
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				texData[x + y * 4] = Vector3(255.f / 2.f, 255.f / 2.f, 255.f);
			}
		}
		s_defaultNormalTexture->Create(texData, texParam);

		delete[] texData;
	}
}

void Texture::BindImageTexture(uint32_t unit, TextureAccess access)
{
	GLCall(glBindImageTexture(unit, m_ID, 0, GL_FALSE, 0, (GLenum)access, GL_R8));
}

void Texture2D::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::Bind(uint32_t p_texture)
{

}

void Texture2D::CreateFromFile(const std::string& p_filePath, const TextureParameters& p_textureParameters)
{
	textureParameters = p_textureParameters;
	textureParameters.textureDataType = TextureDataType::UBYTE;
	glGenTextures(1, &m_ID);

	const uint8_t* imgData = stbi_load(p_filePath.c_str(), &m_Width, &m_Height, &m_Components, 0);
	if (imgData == NULL)
	{
		stbi_image_free((void*)imgData);
		m_ID = DEFAULT_TEX;
		return;
	}

	if (m_Components == 1)
	{
		textureParameters.internalFormat = TextureFormat::R8;
		textureParameters.dataFormat = TextureFormat::RED;
	}

	if (m_Components == 3)
	{
		textureParameters.internalFormat = TextureFormat::RGB8;
		textureParameters.dataFormat = TextureFormat::RGB;
	}
	
	if (m_Components == 4)
	{
		textureParameters.internalFormat = TextureFormat::RGBA8;
		textureParameters.dataFormat = TextureFormat::RGBA;
	}

	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, (int)textureParameters.internalFormat, m_Width, m_Height, 0, (int)textureParameters.dataFormat, (GLenum)textureParameters.textureDataType, imgData);

	if (textureParameters.setTextureParams)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)textureParameters.magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)textureParameters.minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)textureParameters.wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)textureParameters.wrap);
	}

	if (textureParameters.generateMips)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void Texture2D::Create(const void* p_data, const TextureParameters& p_textureParameters)
{
	textureParameters = p_textureParameters;
	glGenTextures(1, &m_ID);
	Bind();
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (int)textureParameters.internalFormat, m_Width, m_Height, 0, (int)textureParameters.dataFormat, (GLenum)textureParameters.textureDataType, p_data));

	if (textureParameters.setTextureParams)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)textureParameters.magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)textureParameters.minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)textureParameters.wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)textureParameters.wrap);
	}

	if (textureParameters.dataFormat == TextureFormat::DEPTH)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, (GLenum)textureParameters.textureCompFunc);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	}

	if (textureParameters.generateMips)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void Texture2D::Create(const TextureParameters& p_textureParameters)
{
	textureParameters = p_textureParameters;
	glGenTextures(1, &m_ID);
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, (int)textureParameters.internalFormat, m_Width, m_Height, 0, (int)textureParameters.dataFormat, (GLenum)textureParameters.textureDataType, nullptr);
	
	if (textureParameters.setTextureParams)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)textureParameters.magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)textureParameters.minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)textureParameters.wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)textureParameters.wrap);
	}

	if (textureParameters.dataFormat == TextureFormat::DEPTH)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, (GLenum)textureParameters.textureCompFunc);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	}
	if (textureParameters.generateMips)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void Texture2D::BufferData(const void* p_data)
{
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, (int)textureParameters.dataFormat, (GLenum)textureParameters.textureDataType, p_data);
	if (textureParameters.generateMips)
		glGenerateMipmap(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void Texture2D::BufferData(const std::string& p_filePath)
{
	const uint8_t* imgData = stbi_load(p_filePath.c_str(), &m_Width, &m_Height, &m_Components, 0);
	if (imgData == nullptr)
	{
		stbi_image_free((void*)imgData);
		return;
	}

	if (m_Components == 3)
	{
		textureParameters.internalFormat = TextureFormat::RGB8;
		textureParameters.dataFormat = TextureFormat::RGB;
	}

	if (m_Components == 4)
	{
		textureParameters.internalFormat = TextureFormat::RGBA8;
		textureParameters.dataFormat = TextureFormat::RGBA;
	}

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, (int)textureParameters.dataFormat, (GLenum)textureParameters.textureDataType, imgData);
	if (textureParameters.generateMips)
		glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void Texture2D::Delete()
{
	glDeleteTextures(1, &m_ID);
}

void Texture3D::Bind()
{
	if (isArrayTexture)
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);
		return;
	}

	glBindTexture(GL_TEXTURE_3D, m_ID);

}

void Texture3D::Bind(uint32_t p_texture)
{

}

void Texture3D::CreateFromFile(const std::string& p_filePath, const TextureParameters& p_textureParameters)
{
	textureParameters = p_textureParameters;
	glGenTextures(1, &m_ID);

	const uint8_t* imgData = stbi_load(p_filePath.c_str(), &m_Width, &m_Height, &m_Components, 0);
	if (imgData == nullptr)
	{
		stbi_image_free((void*)imgData);
		return;
	}

	if (m_Components == 3)
	{
		textureParameters.internalFormat = TextureFormat::RGB8;
		textureParameters.dataFormat = TextureFormat::RGB;
	}
	
	if (m_Components == 4)
	{
		textureParameters.internalFormat = TextureFormat::RGBA8;
		textureParameters.dataFormat = TextureFormat::RGBA;
	}

	Bind();
	GLenum textureTarget = GL_TEXTURE_3D;
	if (isArrayTexture)
		textureTarget = GL_TEXTURE_2D_ARRAY;

	glTexImage3D(textureTarget, 0, (int)textureParameters.internalFormat, m_Width, m_Height, textureParameters.depth, 0, (int)textureParameters.dataFormat, (GLenum)textureParameters.textureDataType, imgData);

	if (textureParameters.setTextureParams)
	{
		glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, (int)textureParameters.magFilter);
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, (int)textureParameters.minFilter);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, (int)textureParameters.wrap);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, (int)textureParameters.wrap);
	}
	
	if (textureParameters.generateMips)
	{
		glGenerateMipmap(textureTarget);
	}

	glBindTexture(textureTarget, GL_NONE);
}

void Texture3D::Create(const void* p_data, const TextureParameters& p_textureParameters)
{
	textureParameters = p_textureParameters;
	glGenTextures(1, &m_ID);
	Bind();
	GLenum textureTarget = GL_TEXTURE_3D;
	if (isArrayTexture)
		textureTarget = GL_TEXTURE_2D_ARRAY;


	glTexImage3D(textureTarget, 0, (int)textureParameters.internalFormat, m_Width, m_Height, textureParameters.depth, 0, (int)textureParameters.dataFormat, (GLenum)textureParameters.textureDataType, p_data);
	if (textureParameters.setTextureParams)
	{
		glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, (int)textureParameters.magFilter);
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, (int)textureParameters.minFilter);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, (int)textureParameters.wrap);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, (int)textureParameters.wrap);
	}

	if (textureParameters.dataFormat == TextureFormat::DEPTH)
	{
		glTexParameteri(textureTarget, GL_TEXTURE_COMPARE_FUNC, (GLenum)textureParameters.textureCompFunc);
		glTexParameteri(textureTarget, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	}
	if (textureParameters.generateMips)
	{
		glGenerateMipmap(textureTarget);
	}
	glBindTexture(textureTarget, GL_NONE);
}

void Texture3D::Create(const TextureParameters& p_textureParameters)
{
	textureParameters = p_textureParameters;
	glGenTextures(1, &m_ID);
	Bind();
	GLenum textureTarget = GL_TEXTURE_3D;
	if (isArrayTexture)
		textureTarget = GL_TEXTURE_2D_ARRAY;

	glTexImage3D(textureTarget, 0, (int)textureParameters.internalFormat, m_Width, m_Height, textureParameters.depth, 0, (int)textureParameters.dataFormat, (GLenum)textureParameters.textureDataType, nullptr);
	if (textureParameters.setTextureParams)
	{
		glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, (int)textureParameters.magFilter);
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, (int)textureParameters.minFilter);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, (int)textureParameters.wrap);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, (int)textureParameters.wrap);
	}

	if (textureParameters.generateMips)
	{
		glGenerateMipmap(textureTarget);
	}
	glBindTexture(textureTarget, GL_NONE);
}

void Texture3D::BufferData(const void* p_data)
{
	GLenum textureTarget = GL_TEXTURE_3D;
	if (isArrayTexture)
		textureTarget = GL_TEXTURE_2D_ARRAY;

	glTexImage3D(textureTarget, 0, (int)textureParameters.internalFormat, m_Width, m_Height, textureParameters.depth, 0, (int)textureParameters.dataFormat, (GLenum)textureParameters.textureDataType, p_data);
	if (textureParameters.generateMips)
		glGenerateMipmap(textureTarget);
	glBindTexture(textureTarget, GL_NONE);
}

void Texture3D::BufferData(const std::string& p_filePath)
{
	const uint8_t* imgData = stbi_load(p_filePath.c_str(), &m_Width, &m_Height, &m_Components, 0);
	if (imgData == nullptr)
	{
		stbi_image_free((void*)imgData);
		return;
	}

	if (m_Components == 3)
	{
		textureParameters.internalFormat = TextureFormat::RGB8;
		textureParameters.dataFormat = TextureFormat::RGB;
	}

	if (m_Components == 4)
	{
		textureParameters.internalFormat = TextureFormat::RGBA8;
		textureParameters.dataFormat = TextureFormat::RGBA;
	}
	GLenum textureTarget = GL_TEXTURE_3D;
	if (isArrayTexture)
		textureTarget = GL_TEXTURE_2D_ARRAY;

	glTexImage3D(textureTarget, 0, (int)textureParameters.internalFormat, m_Width, m_Height, textureParameters.depth, 0, (int)textureParameters.dataFormat, (GLenum)textureParameters.textureDataType, imgData);
	if (textureParameters.generateMips)
		glGenerateMipmap(textureTarget);
	glBindTexture(textureTarget, GL_NONE);
}

void Texture3D::Delete()
{
	glDeleteTextures(1, &m_ID);
}

void TextureCube::Bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}

void TextureCube::Bind(uint32_t p_texture)
{

}

void TextureCube::CreateFromFile(const std::string& p_filePath, const TextureParameters& p_textureParameters)
{
	textureParameters = p_textureParameters;
	glGenTextures(1, &m_ID);

	const uint8_t* imgData = stbi_load(p_filePath.c_str(), &m_Width, &m_Height, &m_Components, 0);
	if (imgData == nullptr)
	{
		stbi_image_free((void*)imgData);
		return;
	}

	if (m_Components == 3)
	{
		textureParameters.internalFormat = TextureFormat::RGB8;
		textureParameters.dataFormat = TextureFormat::RGB;
	}

	if (m_Components == 4)
	{
		textureParameters.internalFormat = TextureFormat::RGBA8;
		textureParameters.dataFormat = TextureFormat::RGBA;
	}

	Bind();
	GLenum textureTarget = GL_TEXTURE_CUBE_MAP;
	for (int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, (int)textureParameters.internalFormat, m_Width, m_Height, 0, (int)textureParameters.dataFormat, (GLenum)textureParameters.textureDataType, imgData);

	if (textureParameters.setTextureParams)
	{
		glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, (int)textureParameters.magFilter);
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, (int)textureParameters.minFilter);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, (int)textureParameters.wrap);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, (int)textureParameters.wrap);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, (int)textureParameters.wrap);
	}

	if (textureParameters.generateMips)
	{
		glGenerateMipmap(textureTarget);
	}
	glBindTexture(textureTarget, GL_NONE);
}

void TextureCube::Create(const void* p_data, const TextureParameters& p_textureParameters)
{
	textureParameters = p_textureParameters;
	glGenTextures(1, &m_ID);
	Bind();
	GLenum textureTarget = GL_TEXTURE_CUBE_MAP;
	for (int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, (int)textureParameters.internalFormat, m_Width, m_Height, 0, (int)textureParameters.dataFormat, (GLenum)textureParameters.textureDataType, p_data);
	if (textureParameters.setTextureParams)
	{
		glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, (int)textureParameters.magFilter);
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, (int)textureParameters.minFilter);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, (int)textureParameters.wrap);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, (int)textureParameters.wrap);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, (int)textureParameters.wrap);
	}

	if (textureParameters.generateMips)
	{
		glGenerateMipmap(textureTarget);
	}
	glBindTexture(textureTarget, GL_NONE);
}

void TextureCube::Create(const TextureParameters& p_textureParameters)
{
	textureParameters = p_textureParameters;
	glGenTextures(1, &m_ID);
	Bind();

	GLenum textureTarget = GL_TEXTURE_CUBE_MAP;
	for (int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, (int)textureParameters.internalFormat, m_Width, m_Height, 0, (int)textureParameters.dataFormat, (GLenum)textureParameters.textureDataType, nullptr);

	if (textureParameters.setTextureParams)
	{
		glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, (int)textureParameters.magFilter);
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, (int)textureParameters.minFilter);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, (int)textureParameters.wrap);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, (int)textureParameters.wrap);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, (int)textureParameters.wrap);
	}

	if (textureParameters.generateMips)
	{
		glGenerateMipmap(textureTarget);
	}
	glBindTexture(textureTarget, GL_NONE);
}

void TextureCube::BufferData(const void* p_data)
{
	GLenum textureTarget = GL_TEXTURE_CUBE_MAP;
	for (int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, (int)textureParameters.internalFormat, m_Width, m_Height, 0, (int)textureParameters.dataFormat, (GLenum)textureParameters.textureDataType, p_data);
	if (textureParameters.generateMips)
		glGenerateMipmap(textureTarget);
	glBindTexture(textureTarget, GL_NONE);
}

void TextureCube::BufferData(const std::string& p_filePath)
{
	const uint8_t* imgData = stbi_load(p_filePath.c_str(), &m_Width, &m_Height, &m_Components, 0);
	if (imgData == nullptr)
	{
		stbi_image_free((void*)imgData);
		return;
	}

	if (m_Components == 3)
	{
		textureParameters.internalFormat = TextureFormat::RGB8;
		textureParameters.dataFormat = TextureFormat::RGB;
	}

	if (m_Components == 4)
	{
		textureParameters.internalFormat = TextureFormat::RGBA8;
		textureParameters.dataFormat = TextureFormat::RGBA;
	}
	GLenum textureTarget = GL_TEXTURE_CUBE_MAP;
	
	for (int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, (int)textureParameters.internalFormat, m_Width, m_Height, 0, (int)textureParameters.dataFormat, (GLenum)textureParameters.textureDataType, imgData);
	if (textureParameters.generateMips)
		glGenerateMipmap(textureTarget);
	glBindTexture(textureTarget, GL_NONE);
}

void TextureCube::Delete()
{
	glDeleteTextures(1, &m_ID);
}