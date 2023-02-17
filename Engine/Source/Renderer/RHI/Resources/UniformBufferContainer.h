#pragma once
#include "RHIBufferBase.h"
#include "EASTL/vector.h"
#include "EASTL/shared_ptr.h"
#include "RHIUniformBuffer.h"

// This can either be subclassed by the other specific uniform buffers or it can contain 
// the resource, by composition

class UniformBufferContainer
{
public:

	void AddData(const void* inData, const size_t inSize);
	void UpdateData(const ConstantBufferType inBufferType, const int32_t inBufferNr);
	void Clear();
	void Bind();
	void Unbind();

public:
	eastl::vector<char> UniformsCache;
	eastl::shared_ptr<RHIUniformBuffer> RHIBuffer;
	size_t Counter = 0;
};