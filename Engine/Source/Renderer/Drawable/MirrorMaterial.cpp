#include "Renderer/Drawable/MirrorMaterial.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"

MirrorMaterial::MirrorMaterial() = default;
MirrorMaterial::~MirrorMaterial() = default;

void MirrorMaterial::SetUniforms(const eastl::unordered_map<eastl::string, SelfRegisteringUniform>& inUniformsCache)
{
	RenderMaterial::SetUniforms(inUniformsCache);






}
