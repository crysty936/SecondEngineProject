#pragma once
#include "EASTL/unordered_map.h"
#include "EASTL/string.h"
#include "EASTL/shared_ptr.h"
#include "Renderer/OpenGL/Buffer/VertexArrayObject.h"
#include "Entity/Entity.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Material.h"

// Each Drawable type should take care of it's own drawing.
// Even Instanced meshes can be drawn this way by having a parent that inherits from DrawableBase contain all of them and not having them as exposed children.
// Culling can be done by the Entity's Transform Data.
// TODO: This should be changed so that all gl draw calls are present in the renderer, and the renderer should take care of rendering and grouping things as it sees fit

class DrawableBase : public Entity
{
public:
	DrawableBase();
	virtual ~DrawableBase();

	void Draw(const eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache) const;
	inline void SetVisible(const bool inValue) { bIsVisible = inValue; }

protected:
	/** Draw commands specific for that object  */
	virtual void Draw_Private() const = 0;
	inline void AddRequiredUniform(const eastl::string& inUniformName) { DrawableMaterial.RequiredUniforms.push_back(inUniformName); }

protected:
	Material DrawableMaterial;
	OpenGLShader Shader;

private:
	bool bIsVisible{ true };
};