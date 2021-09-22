#pragma once
#include "FrameBuffer.h"

class GBuffer
{
	std::unique_ptr<FrameBuffer> m_framebuffer;
public:
	GBuffer()
	{
		m_framebuffer = std::make_unique<FrameBuffer>();
		m_framebuffer->CreateTexture();
		m_framebuffer->AttachGBufferTextures();
	}

	void Bind()
	{
		m_framebuffer->Bind(FrameBufferName::GBUFFER);
	}

	~GBuffer(){}
};