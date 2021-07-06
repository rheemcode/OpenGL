#include "../Debug.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
	: m_RendererID(0), m_Count(0)
{
	//GLCall(glGenBuffers(1, &m_RendererID));
}

IndexBuffer::IndexBuffer(IndexBuffer&& ib) noexcept
{
	m_RendererID = ib.m_RendererID;
	m_Count = ib.m_Count;
	ib.m_RendererID = 0;
	ib.m_Count = 0;
}

IndexBuffer::IndexBuffer(const IndexBuffer& ib)
{
	m_RendererID = ib.m_RendererID;
	m_Count = ib.m_Count;
}

IndexBuffer::IndexBuffer(int count)
	:m_Count{count}
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

IndexBuffer::IndexBuffer(const unsigned int* data, int count)
	: m_Count{count}
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::BufferData(const unsigned int* data, int size, int count)
{
	m_Count = count;
	Bind();
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
