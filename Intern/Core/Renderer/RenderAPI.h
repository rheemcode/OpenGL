#pragma once
#include "Buffers/FrameBuffer.h"
#include "Buffers/VertexBuffer.h"

struct RenderAPI
{
	static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	static void SetClearColor(float r, float g, float b, float a);
	static void ClearBuffers();
	static void ClearDepthBuffer();
	static void ClearClearColorBuffer();
	static void ClearStencilBuffer();
	static void EnableVertexAttribArray(Attrib attrib);
	static void DisableVertexAttribArray(Attrib attrib);
	static void EnableCullFace();
	static void DisableCullFace();
	static void CullFrontFace();
	static void CullBackFace();
	static void BindFrameBuffer(uint32_t id);
};
