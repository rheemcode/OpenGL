#pragma once
#include <GL/glew.h>
#include <string>

#define MAX_TEXTURES 32

class Texture
{
	enum InternalFormat
	{
		RGB8 = GL_RGB8,
		RGBA8 = GL_RGBA8
	};
	enum DataFormat
	{
		RGB = GL_RGB,
		RGBA = GL_RGBA,
	};

	unsigned int m_ID[MAX_TEXTURES] = { MAX_TEXTURES + 1 };

	int m_Width, m_Height, m_Components;
	InternalFormat m_InternalFormat;
	DataFormat m_DataFormat;

	int xOffset, yOffset;
	uint32_t textureCount = 0;
	uint32_t levels = 1;

public:
	void Bind();
	void Bind(uint32_t p_val);
	void ActiveTexture(uint32_t count= 0);
	void BindAll();
	void BindCubeMap();
	void UnBind();
	void Delete();
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
	void BufferData(unsigned char* data, int width, int height, DataFormat format);

	static void Screenshot();
	//static void Init();

	bool operator==(const Texture& p_other)
	{
		return m_ID == p_other.m_ID;
	}
	uint32_t AddImage(const std::string& filepath);
	uint32_t AddImage(const std::string& filepath, uint32_t slot);

	void AddCubeMapImage(const std::array<std::string, 6>& p_files);

	Texture(uint32_t count = 1);
	Texture(uint32_t widtth, uint32_t height);
	Texture(const std::string& filename, uint32_t count = 1);
	~Texture();
};