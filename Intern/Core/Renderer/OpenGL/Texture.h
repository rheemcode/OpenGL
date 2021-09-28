#pragma once
#include <GL/glew.h>
#include <string>

#define TEXTURE_MAX_SIZE 0x1000
#define MAX_TEXTURES 32

#define BIND_DEFAULT_TEXTURE() GLCall(glBindTexture(GL_TEXTURE_2D, Texture::s_defaultTexID))

enum class TextureWrap
{
	NONE               = GL_NONE,
	REPEAT             = GL_REPEAT,	
	CLAMP              = GL_CLAMP,
	MIRRORED_REPEAT    = GL_MIRRORED_REPEAT,
	CLAMP_TO_EDGE      = GL_CLAMP_TO_EDGE,
	CLAMP_TO_BORDER    = GL_CLAMP_TO_BORDER
};

enum class TextureFilter 
{
	NONE = GL_NONE,
	LINEAR = GL_LINEAR
};

enum class TextureFormat
{
	NONE      = GL_NONE,
	R8        = GL_R8,
	RGB8      = GL_RGB8,
	RGBA8     = GL_RGBA8,
	RGBA16    = GL_RGBA16,
	SRGB8     = GL_SRGB8,
	SRGBA8    = GL_SRGB8_ALPHA8,
	RGB16AF   = GL_RGBA16F,
	RGB32     = GL_RGB32F,
	RGBA32F   = GL_RGBA32F,
	RGB		  = GL_RGB,
	RGBA      = GL_RGBA,
	DEPTH     = GL_DEPTH_COMPONENT,
	DEPTH24   = GL_DEPTH_COMPONENT24
};

enum class TextureType
{
	COLOR,
	DEPTH,
	DEPTHARRAY,
	CUBE
};

struct TextureParameters
{
	TextureFormat format;
	TextureFilter minFilter;
	TextureFilter magFilter;
	TextureWrap wrap;
	bool srgb = false;

};

class Texture
{
	friend class Renderer;
	friend class Model;

	unsigned int m_ID[MAX_TEXTURES] = { MAX_TEXTURES + 1 };

	int m_Width, m_Height, m_Components;
	InternalFormat m_InternalFormat;
	DataFormat m_DataFormat;

	int xOffset, yOffset;
	uint32_t textureCount = 0;
	uint32_t levels = 1;

	static uint32_t s_defaultTexID;
	static Texture* s_defaultTexure;

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

	static void CreateDefaultTexture();
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

class Texture2D : public Texture
{
};

class Texture3D : public Texture
{
};

class TextureCube : public Texture
{
};

class TextureDepth : public Texture
{
};