#include "DrawableBase.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"

DrawableBase::DrawableBase()
{
}


DrawableBase::~DrawableBase() = default;

void DrawableBase::Draw(const eastl::unordered_map<eastl::string, SelfRegisteringUniform>& inUniformsCache) const
{
	if (!bIsVisible)
		return;

	Shader.Bind();
 
	using uniformsIterator = const eastl::unordered_map<eastl::string, SelfRegisteringUniform>::const_iterator;
	// Register all required uniforms
	for (const eastl::string& thisUniformName : DrawableMaterial.RequiredUniforms)
	{
		const uniformsIterator& iter = inUniformsCache.find(thisUniformName);

		if ( iter == inUniformsCache.end())
		{
			return;
		}

		const SelfRegisteringUniform& selfRegisteringUniform = (*iter).second;
		selfRegisteringUniform.Register(thisUniformName, Shader);
	}

	Draw_Private();

	Shader.UnBind();
}

