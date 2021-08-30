#pragma once
#include "BufferLayout.h"

class GLIB_API VertexBuffer
{
	unsigned int m_RendererID;
	BufferLayout m_bufferLayout;

public:
	void Resize(unsigned int size);
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(unsigned int size);

	void Bind() const;
	void UnBind() const;

	void SetLayout(BufferLayout&& layout)
	{
		m_bufferLayout = std::move(layout);
	}

	const BufferLayout& GetLayout() const {
		return m_bufferLayout;
	}
	void BufferData(const void* data, unsigned int size);
	void BufferSubData(const void* data, int location, unsigned int size);
	
	VertexBuffer();
	VertexBuffer(const VertexBuffer& vb)
	{
		m_RendererID = vb.m_RendererID;
		m_bufferLayout = vb.m_bufferLayout;
	}
	~VertexBuffer();
};

