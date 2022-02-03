#pragma once
#include "Entity/Entity.h"
#include "IDrawable.h"
#include "Renderer/OpenGL/Buffer/VAO.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Renderer/OpenGL/OpenGLTexture.h"

// TODO: Modify drawable so that it gets a map of abstract uniforms 
// which register themselves given the shader, based on the name

class SimpleShapeDrawable : public Entity, public IDrawable
{
public:
	SimpleShapeDrawable(VAO& inVAO, OpenGLShader& inShader);
	SimpleShapeDrawable();
	virtual ~SimpleShapeDrawable();

	virtual void Init() override;
	void Draw() const override;
	inline const eastl::vector<OpenGLTexture>& GetTextures() const { return Textures; }
	inline const OpenGLShader& GetShader() const { return Shader; }
	void AddTexture(OpenGLTexture inTexture);
	void DeleteObject();

protected:
	VAO ObjectVAO;
	OpenGLShader Shader;
	eastl::vector<OpenGLTexture> Textures;
};