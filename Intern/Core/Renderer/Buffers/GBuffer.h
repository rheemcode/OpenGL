#pragma once
#include "FrameBuffer.h"
#include "Shader.h"

class GBuffer
{
	friend class Scene;
	friend class Renderer;
	std::unique_ptr<FrameBuffer> m_framebuffer;
	std::unique_ptr<Shader> m_shader;
	std::unique_ptr<Shader> m_lightingShader;
public:
	GBuffer()
	{
		m_framebuffer = std::make_unique<FrameBuffer>();
		m_framebuffer->CreateTexture();
		m_framebuffer->AttachGBufferTextures();
		m_shader = std::make_unique<Shader>("./Assets/Shaders/gBuffer.glsl");
		m_lightingShader = std::make_unique<Shader>("./Assets/Shaders/gBufferDraw.glsl");
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

	void BindLightingShader()
	{
		m_lightingShader->Bind();
	}

	Shader* GetShader() { return m_shader.get(); }

	~GBuffer(){}
};