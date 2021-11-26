#pragma once
#include "Utils/Debug.h"
#include <GL/glew.h>

enum class GLIB_API AttribDataType : uint32_t
{
	T_FLOAT =  GL_FLOAT,
	T_INT   =  GL_INT,
	T_UINT  =  GL_UNSIGNED_INT,
	T_BYTE  =  GL_UNSIGNED_BYTE
};

enum class GLIB_API Attrib : uint32_t
{
	VERTEXPOSITION,
	NORMAL,
	UV,
	TANGENT,
	BITANGENT,
};

enum class GLIB_API AttribCount : uint32_t
{
	VEC1 = 1,
	VEC2,
	VEC3,
	VEC4
};

struct GLIB_API VertexBufferElement
{
	AttribDataType type;
	Attrib id;
	AttribCount count;
	bool normalized;
	bool enabled;

	static uint32_t GetSizeOfType(AttribDataType type)
	{
		switch (type)
		{
		    case AttribDataType::T_FLOAT: return sizeof(float);
			case AttribDataType::T_INT: return sizeof(int);
			case AttribDataType::T_UINT: return sizeof(unsigned int);
			case AttribDataType::T_BYTE: return sizeof(unsigned char);
		}
		return 0;
	}

	VertexBufferElement(AttribDataType p_type, Attrib p_id, AttribCount p_count, bool p_normalized = 0, bool p_enabled = true)
		: type(p_type), id(p_id), count(p_count), normalized(p_normalized), enabled(p_enabled) {}
	
	VertexBufferElement() = default;
	~VertexBufferElement() = default;
};


GLIBSTORAGE template GLIB_API class std::vector<VertexBufferElement>;

class GLIB_API BufferLayout
{
private:
	std::vector<VertexBufferElement> m_Element;
	unsigned int m_Stride;

public:

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
	void Push(AttribCount count, Attrib id)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(AttribCount count, Attrib id)
	{
		m_Element.push_back({ AttribDataType::T_FLOAT, id, count, false});
		m_Stride += sizeof(float) * static_cast<uint32_t>(count);
	}
	
	template<>
	void Push<unsigned int>(AttribCount count, Attrib id)
	{
		m_Element.push_back({ AttribDataType::T_UINT, id, count, false });
		m_Stride +=	sizeof(unsigned int) * static_cast<uint32_t>(count);
	}

	template<>
	void Push<unsigned char>(AttribCount count, Attrib id)
	{
		m_Element.push_back({AttribDataType::T_BYTE, id, count, false });
		m_Stride += sizeof(char) *  static_cast<uint32_t>(count);
	}

	void CalculateStride()
	{
		m_Stride = 0;
		for (auto& element : m_Element)
		{
			m_Stride += static_cast<uint32_t>(element.count) * VertexBufferElement::GetSizeOfType(element.type);
		}
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Element; }
	inline unsigned int GetStride() const { return m_Stride;  }

};

