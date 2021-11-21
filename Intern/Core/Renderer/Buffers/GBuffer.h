#pragma once
#include "FrameBuffer.h"
#include "Shader.h"

class GBuffer
{
	friend class Scene;
	friend class Renderer;
	std::unique_ptr<Framebuffer> m_framebuffer;
	std::unique_ptr<Shader> m_bufferGenShader;
	std::unique_ptr<Shader> m_bufferRenderShader;

	enum ShaderType { CREATE, USE };
public:
	GBuffer()
	{
		m_framebuffer = std::make_unique<Framebuffer>();
		m_framebuffer->CreateTexture(FramebufferTexture::POSITION);
		m_framebuffer->CreateTexture(FramebufferTexture::COLOR);
		m_framebuffer->CreateTexture(FramebufferTexture::NORMAL);
		m_framebuffer->AttachGBufferTextures();
		m_bufferGenShader = std::make_unique<Shader>("./Assets/Shaders/gBuffer.glsl");
		m_bufferRenderShader = std::make_unique<Shader>("./Assets/Shaders/deffered.glsl");

		
		m_bufferGenShader->Bind();
		
		m_bufferGenShader->SetInt("albedoTex", 0);
		m_bufferGenShader->SetInt("specularTex", 1);
		m_bufferGenShader->SetInt("normalTex", 2);
		
		m_bufferRenderShader->Bind();

		m_bufferRenderShader->SetInt("gPosition", 0);
		m_bufferRenderShader->SetInt("gNormal", 1);
		m_bufferRenderShader->SetInt("gAlbedoSpec", 2);
		m_bufferRenderShader->SetInt("ssao", 3);
	}

	void BindFramebuffer() const
	{
		m_framebuffer->Bind(FramebufferName::GBUFFER);
	}

	void BindFramebuffer(Framebuffer::Buffer type) const
	{
		m_framebuffer->Bind(type, FramebufferName::GBUFFER);
	}

	void BindTexture(FramebufferTexture::Type texture)
	{
		m_framebuffer->BindTexture(texture);
	}

	void BindAllTextures()
	{
		glActiveTexture(GL_TEXTURE0);
		m_framebuffer->BindTexture(FramebufferTexture::POSITION);
		glActiveTexture(GL_TEXTURE1);
		m_framebuffer->BindTexture(FramebufferTexture::NORMAL);
		glActiveTexture(GL_TEXTURE2);
		m_framebuffer->BindTexture(FramebufferTexture::COLOR);
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