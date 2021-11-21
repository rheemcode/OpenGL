#pragma once
#include <GL/glew.h>
#include <string>

#define TEXTURE_MAX_SIZE 0x1000
#define MAX_TEXTURES 32

#define DEFAULT_TEX 1
#define BIND_DEFAULT_TEXTURE() Texture::s_defaultTexture->Bind()
#define BIND_DEFUALT_NORMALTEXTURE()

enum class TextureWrap : int32_t
{
	NONE               = GL_NONE,
	REPEAT             = GL_REPEAT,	
	CLAMP              = GL_CLAMP,
	MIRRORED_REPEAT    = GL_MIRRORED_REPEAT,
	CLAMP_TO_EDGE      = GL_CLAMP_TO_EDGE,
	CLAMP_TO_BORDER    = GL_CLAMP_TO_BORDER
};

enum class TextureDataType : uint32_t
{
	FLOAT   = GL_FLOAT,
	INT     = GL_INT,
	UINT    = GL_UNSIGNED_INT,
	BYTE    = GL_BYTE,
	UBYTE   = GL_UNSIGNED_BYTE
};

enum class TextureFilter 
{
	NONE    = GL_NONE,
	LINEAR  = GL_LINEAR,
	LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
	NEAREST_MIPMAP_NEAREAST = GL_NEAREST_MIPMAP_NEAREST,
	NEAREST = GL_NEAREST
};


enum class TextureFormat
{
	NONE      = GL_NONE,
	R8        = GL_R8,
	R         = GL_R,
	RED        = GL_RED,
	RG		  = GL_RG,
	RG8       = GL_RG8,
	RGB8      = GL_RGB8,
	RGB16     = GL_RGB16,
	RGB16F     = GL_RGB16F,
	RGBA8     = GL_RGBA8,
	RGBA16    = GL_RGBA16,
	SRGB8     = GL_SRGB8,
	SRGBA8    = GL_SRGB8_ALPHA8,
	RGBA16F  = GL_RGBA16F,
	RGB32F     = GL_RGB32F,
	RGBA32F   = GL_RGBA32F,
	RGB		  = GL_RGB,
	RGBA      = GL_RGBA,
	DEPTH     = GL_DEPTH_COMPONENT,
	DEPTH24   = GL_DEPTH_COMPONENT24,
	DEPTH32   = GL_DEPTH_COMPONENT32
};

enum class TextureType
{
	COLOR,
	DEPTH,
	DEPTHARRAY,
	CUBE
};

enum class TextureCompareFunc
{
	LEQUAL   = GL_LEQUAL,
	GEQUAL   = GL_GEQUAL,
	LESS     = GL_LESS,
	GREATER  = GL_GREATER,
	EQUAL    = GL_EQUAL,
	NOTEQUAL = GL_NOTEQUAL,
	ALWAYS   = GL_ALWAYS,
	NEVER    = GL_NEVER
};

enum class TextureAccess
{
	READ_ONLY  = GL_READ_ONLY,
	WRITE_ONLY  = GL_WRITE_ONLY,
	READ_WRITE = GL_READ_WRITE
};

struct TextureParameters
{
	TextureFormat internalFormat;
	TextureFormat dataFormat;
	TextureFilter minFilter;
	TextureFilter magFilter;
	TextureDataType textureDataType;
	TextureWrap wrap;
	TextureCompareFunc textureCompFunc;

	int depth = 0;
	bool srgb = false;
	bool setTextureParams = true;
	bool generateMips = true;
};

class Texture
{
	friend class Renderer;
	friend class Framebuffer;
	friend class Model;
protected:
	unsigned int m_ID = 0;
	uint32_t textureCount = 0;

	int m_Width, m_Height, m_Components;
	int xOffset, yOffset;

	std::string filePath;
	TextureParameters textureParameters;
	
	static std::shared_ptr<Texture> s_defaultTexture;
	static std::shared_ptr<Texture> s_defaultSpecularTexture;
	static std::shared_ptr<Texture> s_defaultNormalTexture;

public:
	enum : uint32_t
	{
		TEXTURE0   = GL_TEXTURE0,
		TEXTURE1   = GL_TEXTURE1,
		TEXTURE2   = GL_TEXTURE2,
		TEXTURE3   = GL_TEXTURE3,
		TEXTURE4   = GL_TEXTURE5,
		TEXTURE5   = GL_TEXTURE5,
		TEXTURE6   = GL_TEXTURE6,
		TEXTURE7   = GL_TEXTURE7,
		TEXTURE8   = GL_TEXTURE8,
		TEXTURE9   = GL_TEXTURE9,
		TEXTURE10  = GL_TEXTURE10,
		TEXTURE11  = GL_TEXTURE11,
	};

public:
	virtual void Bind() = 0;
	virtual void Bind(uint32_t p_val) = 0;
	virtual void CreateFromFile(const std::string& p_filePath, const TextureParameters& p_textureParameters) = 0;
	virtual void Create(const TextureParameters& p_textureParameters) = 0;
	virtual void Create(const void* p_data, const TextureParameters& p_textureParameters) = 0;
	virtual void BufferData(const void* p_data) = 0;
	virtual void BufferData(const std::string& p_filePath) = 0;
	virtual void Delete() { glDeleteTextures(1, &m_ID); };

	void BindImageTexture(uint32_t unit, TextureAccess access);

	static void ActiveTexture(uint32_t p_texture) { glActiveTexture(p_texture); }
	static void CreateDefaultTextures();

	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }

	void SetWidth(int p_width) { m_Width = p_width; }
	void SetHeight(int p_height) { m_Height = p_height; }
	
	virtual ~Texture() {};
};

class Texture2D : public Texture
{
public:
	virtual void Bind();
	virtual void Bind(uint32_t p_texture);
	virtual void CreateFromFile(const std::string& p_filePath, const TextureParameters& p_textureParameters);
	virtual void Create(const TextureParameters& p_textureParameters);
	virtual void Create(const void* p_data, const TextureParameters& p_textureParameters);
	virtual void BufferData(const void* p_data);
	virtual void BufferData(const std::string& p_filePath);
	virtual void Delete();

	Texture2D() = default;
	Texture2D(const std::string& p_filePath, const TextureParameters& p_textureParameters)
	{
		CreateFromFile(p_filePath, p_textureParameters);
	}
	~Texture2D()
	{
		Delete();
	}
};

class Texture3D : public Texture
{
	bool isArrayTexture = false;
public:
	virtual void Bind();
	virtual void Bind(uint32_t p_texture);
	virtual void CreateFromFile(const std::string& p_filePath, const TextureParameters& p_textureParameters);
	virtual void Create(const TextureParameters& p_textureParameters);
	virtual void Create(const void* p_data, const TextureParameters& p_textureParameters);
	virtual void BufferData(const void* p_data);
	virtual void BufferData(const std::string& p_filePath);
	virtual void Delete();

	void SetDepth(uint32_t p_depth);
	void SetAsArrayTexture(bool p_val) { isArrayTexture = p_val; }

	Texture3D() = default;
	Texture3D(const std::string& p_filePath, const TextureParameters& p_textureParameters)
	{
		CreateFromFile(p_filePath, p_textureParameters);
	}
	~Texture3D()
	{
		Delete();
	}
};

class TextureCube : public Texture
{
public:
	virtual void Bind();
	virtual void Bind(uint32_t p_texture);
	virtual void CreateFromFile(const std::string& p_filePath, const TextureParameters& p_textureParameters);
	void CreateFromFile(const std::string* p_filePath, const TextureParameters& p_textureParameters);
	virtual void Create(const TextureParameters& p_textureParameters);
	virtual void Create(const void* p_data, const TextureParameters& p_textureParameters);
	virtual void BufferData(const void* p_data);
	virtual void BufferData(const std::string& p_filePath);
	virtual void Delete();
};

//class TextureDepth : public Texture
//{
//public:
//	virtual void Bind();
//	virtual void Bind(uint32_t p_texture);
//	virtual void CreateFromFile(const std::string& p_filePath, const TextureParameters& p_textureParameters);
//	virtual void Create(const TextureParameters& p_textureParameters);
//	virtual void BufferData(void* p_data);
//	virtual void BufferData(const std::string& p_filePath);
//	virtual void Delete();
//};