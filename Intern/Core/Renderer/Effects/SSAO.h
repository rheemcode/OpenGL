#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Buffers/FrameBuffer.h"

class SSAO
{
	std::shared_ptr<FrameBuffer> m_ssaoFbo;

public:
	SSAO();
};

