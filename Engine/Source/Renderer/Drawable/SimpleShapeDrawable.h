#pragma once
#include "Entity/Entity.h"
#include "Renderer/Drawable/DrawableBase.h"
#include "Renderer/OpenGL/Buffer/VAO.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Renderer/OpenGL/OpenGLTexture.h"

class SimpleShapeDrawable : public DrawableBase
{
public:
	SimpleShapeDrawable(VAO& inVAO, OpenGLShader& inShader);
	SimpleShapeDrawable();
	virtual ~SimpleShapeDrawable();

	virtual void Init() override;
	inline const eastl::vector<OpenGLTexture>& GetTextures() const { return Textures; }
	inline const OpenGLShader& GetShader() const { return Shader; }
	void AddTexture(OpenGLTexture inTexture);
	void DeleteObject();

protected:
	virtual void Draw_Private() const override;

protected:
	eastl::vector<OpenGLTexture> Textures;


};