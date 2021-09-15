#include <glpch.h>
#include "UniformBuffer.h"
#include "UniformBuffer.h"
#include "Debug.h"


UniformBuffer::UniformBuffer(uint32_t size, const void* data, uint32_t binding)
{
	GLCall(glGenBuffers(1, &m_ID));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW));
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ID));
}

UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding)
{
	GLCall(glGenBuffers(1, &m_ID));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ID));
}

UniformBuffer::UniformBuffer(uint32_t size, uint32_t offset, uint32_t binding)
{
	GLCall(glGenBuffers(1, &m_ID));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ID));
}

UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding, uint32_t offset, const void* data)
{
	GLCall(glGenBuffers(1, &m_ID));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
	GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, binding, m_ID, offset, size));
}

void UniformBuffer::SetData(uint32_t size, const void* data, uint32_t offset)
{
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_ID);
}