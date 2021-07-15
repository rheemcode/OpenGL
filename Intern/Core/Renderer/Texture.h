#pragma once
#include <GL/glew.h>
#include <string>

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

	unsigned int m_ID;
	uint32_t* m_Textures;
	static uint32_t texCount;
	static uint32_t texturesIDs[32];
	int m_Width, m_Height, m_Components;
	InternalFormat m_InternalFormat;
	DataFormat m_DataFormat;

public:
	void Bind();
	void Bind(uint32_t p_val);
	void UnBind();
	void Delete();
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
	void BufferData(unsigned char* data, int width, int height, DataFormat format);

	//static void Init();
	
	bool operator==(const Texture& p_other)
	{
		return m_ID == p_other.m_ID;
	}
	void AddImage(const std::string& filepath);
	void AddImage(const std::string& filepath, uint32_t slot);

	Texture(uint32_t count = 1);
	Texture(uint32_t widtth, uint32_t height);
	Texture(const std::string& filename, uint32_t count = 1);
	~Texture();
};