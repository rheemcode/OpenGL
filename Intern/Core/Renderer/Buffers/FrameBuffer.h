#pragma once
#include "Utils/Debug.h"
#include "Texture.h"
#include "Window/Display.h"

namespace FramebufferName
{
	enum Type
	{
		COLORBUFFER,
		DEPTH,
		GBUFFER,
		MAX
	};
}

namespace FramebufferTexture
{
	enum Type
	{
		COLOR,
		SHADOWMAP,
		SHADOWMAPARRAY,
		CUBEMAPDEPTH,
		POSITION,
		NORMAL,
		MAX
	};
}

class GLIB_API Framebuffer
{
	bool isDepthOnly = true;
	uint32_t fboWidth, fboHeight;
	uint32_t fboID[FramebufferName::MAX];
	uint32_t levels;
	std::shared_ptr<Texture> textures[FramebufferTexture::MAX];

public:
	enum Buffer
	{
		READ = GL_READ_FRAMEBUFFER,
		DRAW = GL_DRAW_FRAMEBUFFER
	};

public:
	void CreateTexture(FramebufferTexture::Type name)
	{
		switch (name)
		{
			case FramebufferTexture::COLOR:
			{
				textures[FramebufferTexture::COLOR] = std::make_shared<Texture2D>();
				return;
			}
			case FramebufferTexture::POSITION:
			{
				textures[FramebufferTexture::POSITION] = std::make_shared<Texture2D>();
				return;
			}
			case FramebufferTexture::NORMAL:
			{
				textures[FramebufferTexture::NORMAL] = std::make_shared<Texture2D>();
				return;
			}
			case FramebufferTexture::SHADOWMAP:
			{
				textures[FramebufferTexture::SHADOWMAP] = std::make_shared<Texture2D>();
				return;
			}
			case FramebufferTexture::SHADOWMAPARRAY:
			{
				textures[FramebufferTexture::SHADOWMAPARRAY] = std::make_shared<Texture3D>();
				return;
			}
			case FramebufferTexture::CUBEMAPDEPTH:
			{
				textures[FramebufferTexture::CUBEMAPDEPTH] = std::make_shared<TextureCube>();
				return;
			}

			default: return;
		}
	}

	int GetFramebufferWidth() const { return fboWidth; }
	int GetFramebufferHeight() const { return fboHeight; }
	Vector2 GetFramebufferSize() const { return Vector2((float)fboWidth, (float)fboHeight); }

	void AttachArrayTexture(int width = 2048, int height = 2048, uint32_t p_levels = 4)
	{
		TextureParameters texParam;
		texParam.internalFormat = TextureFormat::DEPTH32;
		texParam.dataFormat = TextureFormat::DEPTH;
		texParam.magFilter = TextureFilter::NEAREST;
		texParam.minFilter = TextureFilter::NEAREST;
		texParam.textureDataType = TextureDataType::FLOAT;
		texParam.textureCompFunc = TextureCompareFunc::LEQUAL;
		texParam.wrap = TextureWrap::CLAMP_TO_EDGE;
		texParam.depth = p_levels;
		texParam.setTextureParams = true;
		texParam.generateMips = false;

		Texture3D* texture = (Texture3D*)textures[FramebufferTexture::SHADOWMAPARRAY].get();
		texture->SetAsArrayTexture(true);
		texture->SetWidth(width);
		texture->SetHeight(height);
		texture->Create(nullptr, texParam);

		glBindFramebuffer(GL_FRAMEBUFFER, fboID[FramebufferName::DEPTH]);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textures[FramebufferTexture::SHADOWMAPARRAY]->m_ID, 0);
		glReadBuffer(GL_NONE);
		glDrawBuffer(GL_NONE);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Console::Log(LogMode::DEBUG, "Framebuffer Setup Not Complete");
			glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
			glBindTexture(GL_TEXTURE_2D, GL_NONE);
			DebugBreak();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	void AttachDepthTexture(int width = 2048, int height = 2048)
	{
		TextureParameters texParam;
		texParam.internalFormat = TextureFormat::DEPTH32;
		texParam.dataFormat = TextureFormat::DEPTH;
		texParam.textureDataType = TextureDataType::FLOAT;
		texParam.textureCompFunc = TextureCompareFunc::LEQUAL;
		texParam.wrap = TextureWrap::CLAMP_TO_EDGE;
		texParam.setTextureParams = true;
		texParam.generateMips = false;

		Texture2D* texture = (Texture2D*)textures[FramebufferTexture::COLOR].get();
		texture->SetWidth(width);
		texture->SetHeight(height);
		texture->Create(nullptr, texParam);

		fboWidth = width;
		fboHeight = height;

		glBindFramebuffer(GL_FRAMEBUFFER, fboID[FramebufferName::DEPTH]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[FramebufferTexture::SHADOWMAP]->m_ID, 0);
		if (isDepthOnly)
		{
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void AttachRenderBuffer()
	{
		uint32_t rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, GLsizei(Display::GetSingleton()->GetScreenSize(0).x), Display::GetSingleton()->GetScreenSize(0).y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Console::Log(LogMode::DEBUG, "Framebuffer Setup Not Complete");
		}
	}

	void AttachCubeMapTexture(int width, int height)
	{
		TextureParameters texParam;
		texParam.internalFormat = TextureFormat::DEPTH32;
		texParam.dataFormat = TextureFormat::DEPTH;
		texParam.textureDataType = TextureDataType::FLOAT;
		texParam.textureCompFunc = TextureCompareFunc::LEQUAL;
		texParam.wrap = TextureWrap::CLAMP_TO_EDGE;
		texParam.setTextureParams = true;
		texParam.generateMips = false;

		Texture* texture = textures[FramebufferTexture::COLOR].get();
		texture->SetWidth(width);
		texture->SetHeight(height);
		texture->Create(nullptr, texParam);

		glBindFramebuffer(GL_FRAMEBUFFER, fboID[FramebufferName::DEPTH]);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textures[FramebufferTexture::CUBEMAPDEPTH]->m_ID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindBuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	void AttachColorTexture()
	{
		TextureParameters texParam;
		texParam.internalFormat = TextureFormat::RGBA16F;
		texParam.dataFormat = TextureFormat::RGBA;
		texParam.textureDataType = TextureDataType::FLOAT;
		texParam.setTextureParams = false;
		texParam.generateMips = false;

		Texture2D* texture = (Texture2D*)textures[FramebufferTexture::COLOR].get();
		texture->SetWidth(Display::GetSingleton()->GetScreenSize(0).x);
		texture->SetHeight(Display::GetSingleton()->GetScreenSize(0).y);
		texture->Create(nullptr, texParam);

		glBindFramebuffer(GL_FRAMEBUFFER, fboID[FramebufferName::COLORBUFFER]);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textures[FramebufferTexture::COLOR]->m_ID, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	void AttachColorTexture(const TextureParameters& texParams)
	{
		Texture2D* texture = (Texture2D*)textures[FramebufferTexture::COLOR].get();
		texture->SetWidth(Display::GetSingleton()->GetScreenSize(0).x);
		texture->SetHeight(Display::GetSingleton()->GetScreenSize(0).y);
		texture->Create(nullptr, texParams);

		glBindFramebuffer(GL_FRAMEBUFFER, fboID[FramebufferName::COLORBUFFER]);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textures[FramebufferTexture::COLOR]->m_ID, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	void AttachMultipleColorTexture(int count)
	{

	}

	void AttachGBufferTextures()
	{
		Bind(FramebufferName::GBUFFER);
		Texture* texture;
		TextureParameters texParam;
		texParam.internalFormat = TextureFormat::RGBA16F;
		texParam.dataFormat = TextureFormat::RGBA;
		texParam.textureDataType = TextureDataType::FLOAT;
		texParam.minFilter = TextureFilter::NEAREST;
		texParam.magFilter = TextureFilter::NEAREST;
		texParam.wrap = TextureWrap::CLAMP_TO_EDGE;
		texParam.generateMips = false;

		texture = textures[FramebufferTexture::POSITION].get();
		texture->SetWidth(Display::GetSingleton()->GetScreenSize(0).x);
		texture->SetHeight(Display::GetSingleton()->GetScreenSize(0).y);
		texture->Create(nullptr, texParam);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[FramebufferTexture::POSITION]->m_ID, 0);

		texParam.wrap = TextureWrap::CLAMP_TO_EDGE;
		texture = textures[FramebufferTexture::NORMAL].get();
		texture->SetWidth(Display::GetSingleton()->GetScreenSize(0).x);
		texture->SetHeight(Display::GetSingleton()->GetScreenSize(0).y);
		texture->Create(nullptr, texParam);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, textures[FramebufferTexture::NORMAL]->m_ID, 0);

		texParam.internalFormat = TextureFormat::RGBA8;
		texParam.wrap = TextureWrap::NONE;
		texture = textures[FramebufferTexture::COLOR].get();
		texture->SetWidth(Display::GetSingleton()->GetScreenSize(0).x);
		texture->SetHeight(Display::GetSingleton()->GetScreenSize(0).y);
		texture->Create(nullptr, texParam);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, textures[FramebufferTexture::COLOR]->m_ID, 0);


		uint32_t attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		GLCall(glDrawBuffers(3, attachments));

		AttachRenderBuffer();

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Console::Log(LogMode::DEBUG, "Framebuffer Setup Not Complete");
			glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
			glBindTexture(GL_TEXTURE_2D, GL_NONE);
			DebugBreak();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}

	void Bind(Buffer type, FramebufferName::Type name) const
	{
		glBindFramebuffer(type, fboID[name]);
	}
	void Bind(FramebufferName::Type name) const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fboID[name]));
	}

	void BindTexture(FramebufferTexture::Type name) const
	{
		textures[name]->Bind();
	}

	void TextureLayer(FramebufferTexture::Type name, uint32_t layer)  const
	{
		GLCall(glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textures[name]->m_ID, 0, layer));
	}
	

	Framebuffer()
	{
		glGenFramebuffers(FramebufferName::MAX, fboID);

	}

	~Framebuffer()
	{
		glDeleteFramebuffers(FramebufferName::MAX, fboID); 
	}
};