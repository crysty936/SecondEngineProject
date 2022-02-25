#pragma once
#include "EASTL/unordered_map.h"
#include "EASTL/string.h"
#include "EASTL/shared_ptr.h"
#include "Renderer/OpenGL/Buffer/VAO.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Entity/Entity.h"


// Each Drawable type should take care of it's own drawing.
// Even Instanced meshes can be drawn this way by having a parent that inherits from DrawableBase contain all of them and not having them as exposed children.
// Culling can be done by the Entity's Transform Data.
// TODO: This should be changed so that all gl draw calls are present in the renderer, and the renderer should take care of rendering and grouping things as it sees fit

class DrawableBase : public Entity
{
public:
	DrawableBase();
	DrawableBase(VAO& inVAO, OpenGLShader& inShader);
	virtual ~DrawableBase();

	void Draw(const eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache) const;

protected:
	virtual void Draw_Private() const;
	inline const eastl::vector<eastl::string>& GetRequiredUniforms() { return RequiredUniforms; }
	inline void AddRequiredUniform(const eastl::string& inUniformName) { RequiredUniforms.push_back(inUniformName); }

protected:
	VAO ObjectVAO;
	OpenGLShader Shader;

private:
	eastl::vector<eastl::string> RequiredUniforms;
};