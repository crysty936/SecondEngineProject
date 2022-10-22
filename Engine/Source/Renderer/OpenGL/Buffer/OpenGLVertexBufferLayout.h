#pragma once
#include "EASTL/vector.h"
#include "OpenGLLayoutProperties.h"
#include "glad/glad.h"

class OpenGLVertexBufferLayout
{

public:
	OpenGLVertexBufferLayout()
		:Stride(0) {}


	template<typename T>
	void Push(uint32_t Count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(uint32_t Count)
	{
		Properties.push_back({ GL_FLOAT, Count, GL_FALSE });
		Stride += Count * sizeof(float);
	}

	template<>
	void Push<uint32_t>(uint32_t Count)
	{
		Properties.push_back({ GL_UNSIGNED_INT, Count, GL_FALSE });
		Stride += Count * sizeof(uint32_t);
	}

	

	inline const eastl::vector<OpenGLLayoutProperties>& GetProperties() const { return Properties; }
	inline const uint32_t GetStride() const { return Stride; }

private:
	eastl::vector<OpenGLLayoutProperties> Properties;
	uint32_t Stride;
};
