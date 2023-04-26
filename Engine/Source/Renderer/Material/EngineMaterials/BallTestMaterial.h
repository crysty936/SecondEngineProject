#pragma once
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "Renderer/Material/RenderMaterial.h"

class BallTestMaterial : public RenderMaterial
{
public:
	BallTestMaterial();
	~BallTestMaterial();

	void SetUniformsValue(eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache) override;

	virtual void SetRequiredUniforms() override;

};