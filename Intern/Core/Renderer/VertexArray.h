#pragma once


#include "Buffers/BufferLayout.h"
#include "Buffers/VertexBuffer.h"
#include <Renderer/Buffers/IndexBuffer.h>
#include <memory>

class VertexArray
{
	std::unique_ptr<IndexBuffer> m_Ib;
	uint32_t m_RendererID;
public:

	void SetIndexBuffer(IndexBuffer&& p_ib);
	void SetIndices(const uint32_t* data, uint32_t size);
	uint32_t GetIndicies() const;
	void Bind() const;
	void UnBind() const;
	void AddBuffer(const class VertexBuffer& vb, const class BufferLayout& layout);
	void AddBuffer(const class VertexBuffer& vb);

	VertexArray();
	VertexArray(VertexArray&& va) noexcept
	{
		m_RendererID = va.m_RendererID;
		m_Ib = std::move(va.m_Ib);
	}
	~VertexArray();
};
