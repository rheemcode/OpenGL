#pragma once
#include "FrameBuffer.h"
#include "Shader.h"

class GBuffer
{
	std::unique_ptr<FrameBuffer> m_framebuffer;
	std::unique_ptr<Shader> m_shader;
public:
	GBuffer()
	{
		m_framebuffer = std::make_unique<FrameBuffer>();
		m_framebuffer->CreateTexture();
		m_framebuffer->AttachGBufferTextures();
		m_shader = std::make_unique<Shader>("./Assets/Shaders/gBuffer.glsl");
	}

	void BindFramebuffer()
	{
		m_framebuffer->Bind(FrameBufferName::GBUFFER);
	}

	void BindAllTextures()
	{
		glActiveTexture(GL_TEXTURE0);
		m_framebuffer->BindTexture(FrameBufferTexture::POSITION);
		glActiveTexture(GL_TEXTURE1);
		m_framebuffer->BindTexture(FrameBufferTexture::NORMAL);
		glActiveTexture(GL_TEXTURE2);
		m_framebuffer->BindTexture(FrameBufferTexture::COLOR);
	}

	void BindShader()
	{
		m_shader->Bind();
	}

	Shader* GetShader() { return m_shader.get(); }

	~GBuffer(){}
};