#pragma once


#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

class VertexArray
{

private:
	unsigned int m_RendererID;

public:
	VertexArray();
	~VertexArray();
	void Bind() const;
	void UnBind() const;
	void AddBuffer(const class VertexBuffer& vb, const class VertexBufferLayout& layout);

};

