#pragma once
#include "Renderer/RHI/RHI.h"
#include "EASTL/string.h"

class D3D12RHI : public RHI
{
public:
	D3D12RHI();
	~D3D12RHI();


	void WaitForPreviousFrame();
	void MoveToNextFrame();
	void Test() override;

};