#pragma once
#include "Entity/Entity.h"
#include "Renderer/OpenGL/Buffer/VAO.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Renderer/OpenGL/OpenGLTexture.h"

class DrawableObject : public Entity
{
public:
	DrawableObject(VAO& inVAO, OpenGLShader& inShader);
	DrawableObject();
	virtual ~DrawableObject();

	virtual void Init() override;
	void Draw() const;
	inline const eastl::vector<OpenGLTexture>& GetTextures() const { return Textures; }
	inline const OpenGLShader& GetShader() const { return Shader; }
	void AddTexture(OpenGLTexture inTexture);
	void DeleteObject();



protected:
	VAO ObjectVAO;
	OpenGLShader Shader;
	eastl::vector<OpenGLTexture> Textures;
};