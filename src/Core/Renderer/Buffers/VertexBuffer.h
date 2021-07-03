#pragma once


class VertexBuffer
{
	unsigned int m_RendererID;
public:
	VertexBuffer();
	void Resize(unsigned int size);
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;

	void BufferData(const void* data, unsigned int size);
	void BufferSubData(const void* data, int location, unsigned int size);
	
};

