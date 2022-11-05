#pragma once
#include "RHIBufferBase.h"
#include "EASTL/vector.h"
#include "EASTL/shared_ptr.h"

// This can either be subclassed by the other specific uniform buffers or it can contain 
// the resource, by composition

class UniformBufferContainer
{
public:

	void AddData(const char* inData, const size_t inSize);
	void UpdateData();
	void Clear();
	void Bind();
	void Unbind();

public:
	eastl::vector<char> UniformsCache;
	eastl::shared_ptr<class RHIUniformBuffer> RHIBuffer;
	size_t Counter = 0;
};