#include "glpch.h"
#include "RenderAPI.h"

void RenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

void RenderAPI::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void RenderAPI::ClearBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderAPI::ClearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void RenderAPI::ClearClearColorBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderAPI::ClearStencilBuffer()
{
	glClear(GL_STENCIL_BUFFER_BIT);
}

void RenderAPI::EnableVertexAttribArray(Attrib attrib)
{
	glEnableVertexAttribArray(static_cast<uint32_t>(attrib));
}

void RenderAPI::DisableVertexAttribArray(Attrib attrib)
{
	glDisableVertexAttribArray(static_cast<uint32_t>(attrib));
}

void RenderAPI::EnableCullFace()
{
	glEnable(GL_CULL_FACE);
}

void RenderAPI::DisableCullFace()
{
	glDisable(GL_CULL_FACE);
}

void RenderAPI::CullFrontFace()
{
	glCullFace(GL_FRONT);
}

void RenderAPI::CullBackFace()
{
	glCullFace(GL_BACK);
}

void RenderAPI::BindFrameBuffer(uint32_t id)
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}