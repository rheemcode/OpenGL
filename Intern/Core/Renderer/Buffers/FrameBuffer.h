#pragma once
#include "Debug.h"
#include "Window\Window.h"

namespace FrameBufferName
{
	enum Type
	{
		COLORBUFFER,
		DEPTH,
		MAX
	};
}

namespace FrameBufferTexture
{
	enum Type
	{
		COLOR,
		SHADOWMAP,
		MAX
	};
}

class FrameBuffer
{
	uint32_t fboID[FrameBufferName::MAX];
	uint32_t textures[FrameBufferTexture::MAX];

	bool isDepthOnly = true;

public:

	void CreateTexture()
	{
		
		glGenTextures(FrameBufferTexture::MAX, textures);

	}

	void AttachDepthTexture(int width = 2048, int height = 2048)
	{
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
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Display::GetSingleton()->GetMainWindow()->GetWidth(), Display::GetSingleton()->GetMainWindow()->GetHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			Console::Log("Framebuffer Setup Not Complete");
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

	}

	void AttachColorTexture()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[FrameBufferTexture::SHADOWMAP]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, GLint(0), GL_RGBA8, GLsizei(Display::GetSingleton()->GetMainWindow()->GetWidth()), Display::GetSingleton()->GetMainWindow()->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glBindFramebuffer(GL_FRAMEBUFFER, fboID[FrameBufferName::COLORBUFFER]);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textures[FrameBufferTexture::COLOR], 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Bind(FrameBufferName::Type name) const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fboID[name]));
	}

	void BindTexture(FrameBufferTexture::Type name) const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, textures[name]));
	}

	FrameBuffer()
	{
		glGenFramebuffers(FrameBufferName::MAX, fboID);

	}

	~FrameBuffer() { glDeleteFramebuffers(FrameBufferTexture::MAX, fboID); }
};