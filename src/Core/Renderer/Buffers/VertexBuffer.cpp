#include "../Debug.h"
#include"VertexBuffer.h"

VertexBuffer::VertexBuffer()
	: m_RendererID(0)
{
	//GLCall(glGenBuffers(1, &m_RendererID));
	//GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Resize(unsigned int size)
{
	Bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
VertexBuffer::VertexBuffer(unsigned int size)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


void VertexBuffer::BufferData(const void* data, unsigned int size)
{
	Bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::BufferSubData(const void* data, int position, unsigned int size)
{
	Bind();
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, position, size, data));
}

