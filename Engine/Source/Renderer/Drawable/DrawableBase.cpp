#include "DrawableBase.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"

DrawableBase::DrawableBase()
{
	AddRequiredUniform("projection");
	AddRequiredUniform("view");
	AddRequiredUniform("model");
}

DrawableBase::DrawableBase(VAO& inVAO, OpenGLShader& inShader)
	: ObjectVAO{ inVAO }, Shader{ inShader }
{
	AddRequiredUniform("projection");
	AddRequiredUniform("view");
	AddRequiredUniform("model");
}

DrawableBase::~DrawableBase() = default;

void DrawableBase::Draw(const eastl::unordered_map<eastl::string, SelfRegisteringUniform>& inUniformsCache) const
{
	Shader.Bind();
 
	using uniformsIterator = const eastl::unordered_map<eastl::string, SelfRegisteringUniform>::const_iterator;
	// Register all required uniforms
	for (const eastl::string& thisUniformName : RequiredUniforms)
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
}

void DrawableBase::Draw_Private() const
{
}
