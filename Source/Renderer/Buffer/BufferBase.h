#pragma once
#include <stdint.h>

class BufferBase
{
public:
	BufferBase() = default;
	virtual ~BufferBase() = default;
	
public:
	virtual void Bind() = 0;
	virtual void Unbind() = 0;

protected:
	uint32_t Handle = 0;
	bool IsBound = false;
};