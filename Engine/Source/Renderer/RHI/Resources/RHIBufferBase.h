#pragma once

enum class RHIBufferType
{
	Unknown,
	Vertex,
	Index,
	Uniform,
};

class RHIBufferBase
{
public:
	RHIBufferBase(RHIBufferType inBufferType);
	virtual ~RHIBufferBase() = default;

	const RHIBufferType BufferType;
};