#pragma once
#include <vector>


class IndexBuffer
{
private:
	unsigned int m_RendererID;
   int m_Count;
	std::vector<unsigned int> m_Data;

public:
	IndexBuffer(int count);
	IndexBuffer(const unsigned int* data, int count);
	~IndexBuffer();


	void AddData(const unsigned int &data);
	void BufferData();


	void Bind() const;
	void UnBind() const;

	inline unsigned int GetCount() const { return m_Count;  }

};

