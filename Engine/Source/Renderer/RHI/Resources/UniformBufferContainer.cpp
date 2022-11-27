#include "UniformBufferContainer.h"
#include "RHIUniformBuffer.h"
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

void UniformBufferContainer::UpdateData(const ConstantBufferType inBufferType)
{
	if (!RHIBuffer)
	{
		// D3D11 expects that the given buffer size is a multiple of 16
		size_t bufferSize = UniformsCache.size();
		if ((bufferSize % 16) > 0)
		{
			const int multiplier = (bufferSize / 16) + 1;
			bufferSize = multiplier * 16;
		}

		RHIBuffer = RHI::Instance->CreateUniformBuffer(bufferSize);
		RHIBuffer->BType = inBufferType;
	}

	RHI::Instance->UniformBufferUpdateData(*RHIBuffer, UniformsCache.data(), UniformsCache.size());
}

void UniformBufferContainer::Clear()
{
	// Reset data counter
	Counter = 0;
}

void UniformBufferContainer::Bind()
{
	RHI::Instance->BindUniformBuffer(*RHIBuffer);
}

void UniformBufferContainer::Unbind()
{
	RHI::Instance->UnbindUniformBuffer(*RHIBuffer);
}
