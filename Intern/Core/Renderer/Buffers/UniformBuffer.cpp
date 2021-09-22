#include <glpch.h>
#include "UniformBuffer.h"
#include "OpenGL/Shader.h"
#include "Utils/Debug.h"
#include <GL/glew.h>

void UniformBuffer::InitData(Shader* shader, const std::string& name)
{
	Bind();
	m_index = glGetUniformBlockIndex(shader->GetProgram(), name.c_str());
	GLCall(glUniformBlockBinding(shader->GetProgram(), m_index, shader->GetUniformBlockBinding(name)));
	glGetActiveUniformBlockiv(shader->GetProgram(), m_index, GL_UNIFORM_BLOCK_DATA_SIZE, &m_size);
	m_data = new uint8_t[m_size];
	GLCall(glBufferData(GL_UNIFORM_BUFFER, m_size, nullptr, GL_DYNAMIC_DRAW));
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, m_index, m_ID));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE));
}

void UniformBuffer::Bind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
}

void UniformBuffer::FlushBuffer()
{
	Bind();
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_size, m_data);
	glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
}

UniformBuffer::UniformBuffer(uint32_t size, const void* data, uint32_t index)
{
	GLCall(glGenBuffers(1, &m_ID));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW));
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, index, m_ID));

	glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
}

UniformBuffer::UniformBuffer()
{
	GLCall(glGenBuffers(1, &m_ID));
}

UniformBuffer::UniformBuffer(uint32_t size, uint32_t index, uint32_t offset, const void* data)
{
	GLCall(glGenBuffers(1, &m_ID));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
	GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, index, m_ID, offset, size));

	glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
}

void UniformBuffer::SetData(uint32_t size, const void* data, uint32_t offset)
{
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_ID);
	delete[] m_data;
}