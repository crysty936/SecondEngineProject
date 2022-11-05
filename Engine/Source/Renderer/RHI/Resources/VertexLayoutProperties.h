#pragma once

#include <stdint.h>
#include "Core/EngineUtils.h"
#include "EASTL/string.h"

enum class VertexPropertyType : uint32_t
{
	UInt,
	Float
};

enum class VertexInputType : int32_t
{
	Undefined = 0,
	Position,
	Normal,
	TexCoords,
	Count
};

struct VertexLayoutProperties
{
	/** Float or UInt */
	VertexPropertyType Type;
	/** 3 for Basic Vector */
	uint32_t Count;

	uint32_t bNormalized;

	VertexInputType InputType = VertexInputType::Undefined;

	uint32_t GetSizeOfType() const;
};


