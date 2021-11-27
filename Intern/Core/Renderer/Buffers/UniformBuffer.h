#pragma once
#include <cstdint>


class UniformBuffer
{
	uint32_t m_ID;
	int32_t m_size;
	uint32_t m_index;
	uint32_t m_binding;
	const char* m_name;
	uint8_t* m_data;

public:
	template<typename T>
	void UploadData(const T& data, const int& offset);

	void InitData(class Shader* shader, const std::string& name);
	void FlushBuffer();
	void SetData(uint32_t size, const void* data, uint32_t offset);
	void Bind();

	UniformBuffer();
	UniformBuffer(uint32_t size, const void* data, uint32_t index);
	UniformBuffer(uint32_t size, uint32_t index, uint32_t offset, const void* data);
	~UniformBuffer();
};

template<typename T>
inline void UniformBuffer::UploadData(const T& p_data, const int& offset)
{
	T* buffer = new (m_data + offset) T;
	*buffer = p_data;
}
