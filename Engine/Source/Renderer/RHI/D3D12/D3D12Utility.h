#pragma once
#include "Renderer/RHI/RHI.h"
#include "EASTL/string.h"
#include <winerror.h>
#include "Core/EngineUtils.h"
#include <d3d12.h>



namespace D3D12Globals
{
	extern ID3D12Device* Device;



}

namespace D3D12Utility
{
	inline bool DXAssert(HRESULT inRez)
	{
		const bool success = SUCCEEDED(inRez);
		ASSERT_MSG(success, "Direct3D12 Operation failed with code 0x%08X", static_cast<uint32_t>(inRez));

		return success;
	}



}