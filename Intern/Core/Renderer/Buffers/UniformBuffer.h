#pragma once
#include <cstdint>


class UniformBuffer
{
	uint32_t m_ID;
public:
	void SetData(uint32_t size, const void* data, uint32_t offset);
	UniformBuffer(uint32_t size, uint32_t binding);
	UniformBuffer(uint32_t size, uint32_t offset, uint32_t binding);
	UniformBuffer(uint32_t size, const void* data, uint32_t binding);
	UniformBuffer(uint32_t size, uint32_t binding, uint32_t offset, const void* data);
	~UniformBuffer();
};
