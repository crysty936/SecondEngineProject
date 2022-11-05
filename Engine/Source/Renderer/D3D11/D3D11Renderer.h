#pragma once
#include "EASTL/unique_ptr.h"
#include "EASTL/shared_ptr.h"
#include "EASTL/vector.h"
#include "EASTL/unordered_map.h"
#include "EASTL/string.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "Math/Transform.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"
#include "EventSystem/EventSystem.h"
#include "EASTL/queue.h"
#include "Core/EngineUtils.h"
#include "Entity/TransformObject.h"
#include "Window/WindowProperties.h"
#include "Window/WindowsWindow.h"

class D3D11Renderer
{
private:
	D3D11Renderer(const WindowProperties& inMainWindowProperties);
	virtual ~D3D11Renderer();

public:
	static void Init(const WindowProperties& inMainWindowProperties = {});
	static void Terminate();
	void Draw();

	inline static D3D11Renderer& Get() { ASSERT(GlobalRHI); return *GlobalRHI; }

private:
	inline static D3D11Renderer* GlobalRHI = nullptr;
};


