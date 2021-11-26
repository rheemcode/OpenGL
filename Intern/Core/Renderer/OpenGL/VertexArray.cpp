#include <glpch.h>
#include "Utils/Debug.h"
#include "VertexArray.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
	GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::SetIndexBuffer(IndexBuffer&& p_ib)
{
	m_Ib = std::make_unique<IndexBuffer>(std::move(p_ib));
}

void VertexArray::SetIndices(const uint32_t* data, uint32_t count)
{
	m_Ib = std::make_unique<IndexBuffer>(data, count);
}

uint32_t VertexArray::GetIndicies() const
{
	return m_Ib->GetCount();
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
	m_Ib->Bind();
}

void VertexArray::UnBind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const BufferLayout& layout)
{
	Bind();
	vb.Bind();

	unsigned int offset = 0;

	const auto& elements = layout.GetElements();

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(static_cast<uint32_t>(element.id)));
		GLCall(glVertexAttribPointer(static_cast<uint32_t>(element.id), static_cast<uint32_t>(element.count), static_cast<uint32_t>(element.type), element.normalized, layout.GetStride(), (void*)offset));
		offset += static_cast<uint32_t>(element.count) * static_cast<uint32_t>(VertexBufferElement::GetSizeOfType(element.type));
	}
}
void VertexArray::AddBuffer(const VertexBuffer& vb)
{
	Bind();
	vb.Bind();

	auto layout = vb.GetLayout();

	unsigned int offset = 0;

	const auto& elements = layout.GetElements();

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		if (element.enabled)
		{
			GLCall(glEnableVertexAttribArray(static_cast<uint32_t>(element.id)));
		}
		else
		{
			GLCall(glDisableVertexAttribArray(static_cast<uint32_t>(element.id)));
		}

		GLCall(glVertexAttribPointer(static_cast<uint32_t>(element.id), static_cast<uint32_t>(element.count), static_cast<uint32_t>(element.type), element.normalized, layout.GetStride(), (void*) offset));
		offset += static_cast<uint32_t>(element.count) * static_cast<uint32_t>(VertexBufferElement::GetSizeOfType(element.type));
	}
}
