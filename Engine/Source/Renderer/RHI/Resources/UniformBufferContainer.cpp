#include "UniformBufferContainer.h"
#include "UniformBufferBase.h"
#include "Renderer/RHI/RHI.h"

void UniformBufferContainer::AddData(const char* inData, const size_t inSize)
{
	if (UniformsCache.size() < (Counter + inSize))
	{
		UniformsCache.resize(Counter + inSize);
	}

	memcpy(&UniformsCache[Counter], inData, inSize);
	Counter += inSize;
}

void UniformBufferContainer::UpdateData()
{
	if (!RHIBuffer)
	{
		RHIBuffer = RHI::Instance->CreateUniformBuffer(UniformsCache.size());
	}
	RHIBuffer->SetData(UniformsCache.data(), UniformsCache.size());
}

void UniformBufferContainer::Clear()
{
	// Reset data counter
	Counter = 0;
}
