#pragma once
#include "Debug.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int id;
	unsigned int count;
	unsigned char normalized;

	VertexBufferElement(uint32_t p_type, uint32_t p_id, uint32_t p_count, uint8_t p_normalized = 0)
		: type(p_type), id(p_id), count(p_count), normalized(p_normalized) {}

	static uint32_t GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return sizeof(float);
			case GL_INT: return sizeof(int);
			case GL_UNSIGNED_INT: return sizeof(unsigned int); 
			case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
		}
		return 0;
	}
};

class BufferLayout
{
private:
	std::vector<VertexBufferElement> m_Element;
	unsigned int m_Stride;

public:
	enum ElementSize : uint32_t
	{
		Float = 1,
		VEC2,
		VEC3,
		VEC4
	};

	BufferLayout()
		: m_Stride{ 0 }
	{

	}

	BufferLayout(std::initializer_list<VertexBufferElement> elems)
		: m_Element(elems), m_Stride(0)
	{
		CalculateStride();
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

	void CalculateStride()
	{
		m_Stride = 0;
		for (auto& element : m_Element)
		{
			m_Stride += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Element; }
	inline unsigned int GetStride() const { return m_Stride;  }

};

