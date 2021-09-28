#pragma once
#include "FrameBuffer.h"
#include "Shader.h"

class GBuffer
{
	friend class Scene;
	friend class Renderer;
	std::unique_ptr<FrameBuffer> m_framebuffer;
	std::unique_ptr<Shader> m_bufferGenShader;
	std::unique_ptr<Shader> m_bufferRenderShader;

	enum ShaderType { CREATE, USE };
public:
	GBuffer()
	{
		m_framebuffer = std::make_unique<FrameBuffer>();
		m_framebuffer->CreateTexture();
		m_framebuffer->AttachGBufferTextures();
		m_bufferGenShader = std::make_unique<Shader>("./Assets/Shaders/gBuffer.glsl");
		m_bufferRenderShader = std::make_unique<Shader>("./Assets/Shaders/deffered.glsl");
	}

	void BindFramebuffer() const
	{
		m_framebuffer->Bind(FrameBufferName::GBUFFER);
	}

	void BindFramebuffer(FrameBuffer::Buffer type) const
	{
		m_framebuffer->Bind(type, FrameBufferName::GBUFFER);
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

	void BindShader(ShaderType type)
	{
		if (type == ShaderType::USE)
		{
			m_bufferRenderShader->Bind();
			return;
		}

		m_bufferGenShader->Bind();
	}

	~GBuffer(){}
};