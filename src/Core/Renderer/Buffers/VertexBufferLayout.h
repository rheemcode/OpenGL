#pragma once
#include <vector>
#include "../Debug.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int id;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return sizeof(float);
			case GL_UNSIGNED_INT: return sizeof(unsigned int); 
			case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
		}
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Element;
	unsigned int m_Stride;

public:
	enum ElementSize : uint32_t
	{
		VEC1 = 1,
		VEC2,
		VEC3,
		VEC4
	};

	VertexBufferLayout()
		: m_Stride{ 0 }
	{

	}

	template<typename T>
	void Push(ElementSize count, unsigned int id)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(ElementSize count, unsigned int id)
	{
		m_Element.push_back({ GL_FLOAT, id, (uint32_t)count, GL_FALSE });
		m_Stride += sizeof(float) * count;
	}
	
	template<>
	void Push<unsigned int>(ElementSize count, unsigned int id)
	{
		m_Element.push_back({ GL_UNSIGNED_INT, id, (uint32_t)count, GL_FALSE });
		m_Stride +=	sizeof(unsigned int) * count;
	}

	template<>
	void Push<unsigned char>(ElementSize count, unsigned int id)
	{
		m_Element.push_back({GL_UNSIGNED_BYTE, id, (uint32_t )count, GL_TRUE });
		m_Stride += sizeof(char) * count;
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Element; }
	inline unsigned int GetStride() const { return m_Stride;  }

};

