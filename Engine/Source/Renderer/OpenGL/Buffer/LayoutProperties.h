#pragma once
#include <stdint.h>
#include "Core/EngineUtils.h"

struct LayoutProperties
{
	/** Float or UInt */
	uint32_t Type;
	/** 3 for Basic Vector */
	uint32_t Count;
	uint32_t bNormalized;

	uint32_t GetSizeOfType() const;
};


