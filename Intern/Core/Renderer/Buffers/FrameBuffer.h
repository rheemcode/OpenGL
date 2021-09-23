#pragma once
#include "Utils/Debug.h"
#include "Window/Display.h"

namespace FrameBufferName
{
	enum Type
	{
		COLORBUFFER,
		DEPTH,
		GBUFFER,
		MAX
	};
}

namespace FrameBufferTexture
{
	enum Type
	{
		COLOR,
		SHADOWMAP,
		CUBEMAPDEPTH,
		POSITION,
		NORMAL,
		MAX
	};
}

class GLIB_API FrameBuffer
{
	uint32_t textureWidth, textureHeight;
	uint32_t fboID[FrameBufferName::MAX];
	uint32_t levels;
	uint32_t textures[FrameBufferTexture::MAX];
	bool isDepthOnly = true;

public:

	void CreateTexture()
	{
		glGenTextures(FrameBufferTexture::MAX, textures);
	}

	void CreateTexuture(FrameBufferTexture::Type name)
	{
		glGenTextures(1, &textures[name]);
	}

	int GetTextureWidth() const { return textureWidth; }
	int GetTextureHeight() const { return textureHeight; }
	Vector2 GetTextureSize() const { return Vector2((float)textureWidth, (float)textureHeight); }

	void AttachArrayTexture(int width = 2048, int height = 2048, uint32_t p_levels = 4)
	{
		glActiveTexture(GL_TEXTURE0);
		BindArrayTexture(FrameBufferTexture::SHADOWMAP);
		levels = p_levels;
		glTexImage3D(GL_TEXTURE_2D_ARRAY, GLint(0), GL_DEPTH_COMPONENT24, width, height, levels, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glBindTexture(GL_TEXTURE_2D_ARRAY, GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, fboID[FrameBufferName::DEPTH]);
		glReadBuffer(GL_NONE);
		glDrawBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	void AttachDepthTexture(int width = 2048, int height = 2048)
	{
		textureWidth = width;
		textureHeight = height;
		glActiveTexture(GL_TEXTURE0);
		BindTexture(FrameBufferTexture::SHADOWMAP);
		glTexImage2D(GL_TEXTURE_2D, GLint(0), GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, fboID[FrameBufferName::DEPTH]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[FrameBufferTexture::SHADOWMAP], 0);
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
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Display::GetSingleton()->GetMainWindow()->GetWidth(), Display::GetSingleton()->GetMainWindow()->GetHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Console::Log(LogMode::DEBUG, "Framebuffer Setup Not Complete");
		}
	}

	void AttachCubeMapTexture(int width, int height)
	{
		glBindTexture(GL_TEXTURE_2D, textures[FrameBufferTexture::CUBEMAPDEPTH]);
		for (uint32_t i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, fboID[FrameBufferName::DEPTH]);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textures[FrameBufferTexture::CUBEMAPDEPTH], 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindBuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	void AttachColorTexture()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[FrameBufferTexture::SHADOWMAP]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, GLint(0), GL_RGBA16F, GLsizei(Display::GetSingleton()->GetMainWindow()->GetWidth()), Display::GetSingleton()->GetMainWindow()->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glBindFramebuffer(GL_FRAMEBUFFER, fboID[FrameBufferName::COLORBUFFER]);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textures[FrameBufferTexture::COLOR], 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void AttachMultipleColorTexture(int count)
	{

	}

	void AttachGBufferTextures()
	{
	//	BindTexture(FrameBufferTexture::SHADOWMAP);
	//	glTexImage2D(GL_TEXTURE_2D, GLint(0), GL_RGBA8, GLsizei(Display::GetSingleton()->GetMainWindow()->GetWidth()), Display::GetSingleton()->GetMainWindow()->GetHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//	glBindTexture(GL_TEXTURE_2D, 0);

	//	glBindFramebuffer(GL_FRAMEBUFFER, fboID[FrameBufferName::GBUFFER]);
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[FrameBufferTexture::SHADOWMAP], 0);
	//	//if (isDepthOnly)
	//	//{
	//	//	glDrawBuffer(GL_NONE);
	//	//	glReadBuffer(GL_NONE);
	//	//}

	////	glBindFramebuffer(GL_FRAMEBUFFER, 0);



		Bind(FrameBufferName::GBUFFER);
		BindTexture(FrameBufferTexture::POSITION);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, GLsizei(Display::GetSingleton()->GetMainWindow()->GetWidth()), Display::GetSingleton()->GetMainWindow()->GetHeight(), 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[FrameBufferTexture::POSITION], 0);

		BindTexture(FrameBufferTexture::NORMAL);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GLsizei(Display::GetSingleton()->GetMainWindow()->GetWidth()), Display::GetSingleton()->GetMainWindow()->GetHeight(), 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, textures[FrameBufferTexture::NORMAL], 0);

		BindTexture(FrameBufferTexture::COLOR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GLsizei(Display::GetSingleton()->GetMainWindow()->GetWidth()), Display::GetSingleton()->GetMainWindow()->GetHeight(), 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, textures[FrameBufferTexture::COLOR], 0);

		
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

	void Bind(FrameBufferName::Type name) const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fboID[name]));
	}

	void BindTexture(FrameBufferTexture::Type name) const
	{
		//glActiveTexture(GL_TEXTURE0);
		GLCall(glBindTexture(GL_TEXTURE_2D, textures[name]));
	}

	void TextureLayer(FrameBufferTexture::Type name, uint32_t layer)  const
	{
		GLCall(glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textures[name], 0, layer));
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Console::Log(LogMode::DEBUG, "Framebuffer Setup Not Complete");
		}
	}
	
	void BindArrayTexture(FrameBufferTexture::Type name) const
	{
		glActiveTexture(GL_TEXTURE0);
		GLCall(glBindTexture(GL_TEXTURE_2D_ARRAY, textures[name]));
	}

	FrameBuffer()
	{
		glGenFramebuffers(FrameBufferName::MAX, fboID);

	}

	~FrameBuffer()
	{
		glDeleteFramebuffers(FrameBufferName::MAX, fboID); 
		glDeleteTextures(FrameBufferTexture::MAX, textures);
	}
};