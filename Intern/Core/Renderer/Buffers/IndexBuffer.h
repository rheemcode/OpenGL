#pragma once
#include "GLCore.h"

class GLIB_API IndexBuffer
{
	unsigned int m_RendererID;
	int m_Count;

public:
	void BufferData(const unsigned int* data, int size, int count);
	void Bind() const;
	void UnBind() const;
	inline unsigned int GetCount() const { return m_Count; }


	IndexBuffer& operator=(const IndexBuffer& ib)
	{
		m_RendererID = ib.m_RendererID;
		m_Count = ib.m_Count;
		return *this;
	}

	IndexBuffer();
	IndexBuffer(int count);
	IndexBuffer(const unsigned int* data, int count);
	IndexBuffer(IndexBuffer&& ib) noexcept;
	IndexBuffer(const IndexBuffer& ib);
	~IndexBuffer();


};

