#pragma once
#include "RHIBufferBase.h"
#include "VertexBufferLayout.h"

class VertexBufferBase : public RHIBufferBase
{
public:
	inline const VertexBufferLayout& GetLayout() const { return Layout; }

private:
	VertexBufferLayout Layout;
};