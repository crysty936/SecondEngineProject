#pragma once
#include <stdint.h>
#include "EASTL/string.h"

class RHITexture2D
{
public:
	virtual void Bind(int32_t inTexNr) = 0;
	virtual void Unbind(int32_t inTexNr) = 0;

public:
	int32_t NrChannels = 0;

	// TODO: Debug only
	eastl::string SourcePath;
};