#pragma once
#include <vector>


class IndexBuffer
{
	unsigned int m_RendererID;
	int m_Count;

public:
	void BufferData(const unsigned int* data, int size, int count);
	void Bind() const;
	void UnBind() const;
	inline unsigned int GetCount() const { return m_Count; }

	IndexBuffer();
	IndexBuffer(int count);
	IndexBuffer(const unsigned int* data, int count);
	~IndexBuffer();


};

