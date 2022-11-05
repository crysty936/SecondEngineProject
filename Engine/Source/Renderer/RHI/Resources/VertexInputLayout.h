#pragma once

#include "EASTL/vector.h"
#include "VertexLayoutProperties.h"

class VertexInputLayout
{

public:
	VertexInputLayout()
		:Stride(0) {}


	template<typename T>
	void Push(uint32_t Count, const VertexInputType inType)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(uint32_t Count, const VertexInputType inType)
	{
		Properties.push_back({ VertexPropertyType::Float, Count, false, inType });
		Stride += Count * sizeof(float);
	}

	template<>
	void Push<uint32_t>(uint32_t Count, const VertexInputType inType)
	{
		Properties.push_back({ VertexPropertyType::UInt, Count, false, inType });
		Stride += Count * sizeof(uint32_t);
	}

	inline const eastl::vector<VertexLayoutProperties>& GetProperties() const { return Properties; }
	inline const uint32_t GetStride() const { return Stride; }

public:
	eastl::vector<VertexLayoutProperties> Properties;
	uint32_t Stride;
};
