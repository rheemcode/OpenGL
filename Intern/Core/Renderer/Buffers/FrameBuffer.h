#pragma once
#include <cstdint>
#include "..\Debug.h"

enum FrameBufferName
{
	FRAMEBUFFER,
	SHADOW
};

enum FrameBufferTexture
{
	COLOR,
	DEPTH,
	SHADOWMAP
};
class FrameBuffer
{
	uint32_t depthFbo[2];
	uint32_t textures[3] = { 0 };
public:
	FrameBuffer()
	{
		GLCall(glGenTextures(3, textures));

		//// color buffer texture
		//GLCall(glActiveTexture(GL_TEXTURE0));
		//GLCall(glBindTexture(GL_TEXTURE_2D, textures[COLOR]));
		//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
		//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0));
		//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1200, 700, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
		//
		//// render buffer texture
		//GLCall(glActiveTexture(GL_TEXTURE0));
		//GLCall(glBindTexture(GL_TEXTURE_2D, textures[DEPTH]));
		//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
		//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0));
		//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 1200, 700, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL));
		//
		//// shadow map texture
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, textures[SHADOWMAP]));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE));
		GLCall(glTexImage2D(GL_TEXTURE_2D, GLint(0), GL_DEPTH_COMPONENT32, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));

		/*GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		glBindTexture(GL_TEXTURE_2D, 0);
		GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthTexture, 0));*/


		//unsigned int rbo;
		//glGenRenderbuffers(1, &rbo);
		//GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
		//GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, 1920, 1080));
		//glBindRenderbuffer(GL_RENDERBUFFER, 1);
		//
		//GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo));


		//LCall(glGenFramebuffers(1, &depthFbo));
		//GLCall(glBindFramebuffer(GL_FRAMEBUFFER, depthFbo));

	//	GLCall(glDrawBuffer(GL_NONE));

		GLenum const BuffersRender = GL_COLOR_ATTACHMENT0;
		GLCall(glGenFramebuffers(2, depthFbo));
		//GLCall(glBindFramebuffer(GL_FRAMEBUFFER, depthFbo[FrameBufferName::FRAMEBUFFER]));
		//GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textures[COLOR], 0));
		//GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textures[DEPTH], 0));
		//GLCall(glDrawBuffers(1, &BuffersRender));

		//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		//	Console::Log("framebuffer not complete");

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, depthFbo[FrameBufferName::SHADOW]));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[SHADOWMAP], 0));
		GLCall(glDrawBuffer(GL_NONE));
		GLCall(glReadBuffer(GL_NONE));

   		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			Console::Log("framebuffer not complete");

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	//	GLCall(glDrawBuffer(GL_BACK));

	}

	void Bind(FrameBufferName name)
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, depthFbo[name]));
	}

	void BindTexture(FrameBufferTexture name)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, textures[SHADOWMAP]));
	}
	~FrameBuffer() { glDeleteFramebuffers(2, depthFbo); }
};

