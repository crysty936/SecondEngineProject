#pragma once
#include <stdint.h>

class BufferBase
{
public:
	BufferBase();
	virtual ~BufferBase();

	virtual void DeleteBuffer();

public:
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

protected:
	uint32_t Handle = 0;
	mutable bool IsBound = false;
};