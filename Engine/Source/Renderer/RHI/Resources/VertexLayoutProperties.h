#pragma once

#include <stdint.h>
#include "Core/EngineUtils.h"

enum class VertexPropertyType : uint32_t
{
	UInt,
	Float
};

struct VertexLayoutProperties
{
	/** Float or UInt */
	VertexPropertyType Type;
	/** 3 for Basic Vector */
	uint32_t Count;
	uint32_t bNormalized;

	uint32_t GetSizeOfType() const;
};


