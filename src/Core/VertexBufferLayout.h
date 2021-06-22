#pragma once
#include <vector>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	int id;
	unsigned int count;

	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4; 
			case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Element;
	unsigned int m_Stride;
public:

	VertexBufferLayout()
		: m_Stride{ 0 }
	{

	}

	template<typename T>
	void Push(unsigned int count, int id)
	{
		static_assert(false);
	}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}


	template<>
	void Push<float>(unsigned int count, int id)
	{
		m_Element.push_back({ GL_FLOAT, id, count, GL_FALSE });
		m_Stride += sizeof(float) * count;
	}
	
	template<>
	void Push<unsigned int>(unsigned int count, int id)
	{
		m_Element.push_back({ GL_UNSIGNED_INT, id, count, GL_FALSE });
		m_Stride +=	sizeof(unsigned int) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count, int id)
	{
		m_Element.push_back({GL_UNSIGNED_BYTE, id, count, GL_TRUE });
		m_Stride += sizeof(char) * count;
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Element; }
	inline unsigned int GetStride() const { return m_Stride;  }

};

