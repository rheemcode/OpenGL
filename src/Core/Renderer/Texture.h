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
	int m_Width, m_Height, m_Components;
	InternalFormat m_InternalFormat;
	DataFormat m_DataFormat;

public:
	void Bind();
	void UnBind();
	void Delete();
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
	void BufferData(unsigned char* data, int width, int height, DataFormat format);

	Texture();
	void AddImage(const std::string& filepath);
	Texture(const std::string& filename);
	~Texture();
};