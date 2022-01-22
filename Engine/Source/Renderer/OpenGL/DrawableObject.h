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
	virtual void Tick(const float inDeltaT) override;
	void Draw();
	inline eastl::vector<OpenGLTexture>& GetTextures(){ return Textures; }
	inline OpenGLShader& GetShader() { return Shader; }
	void AddTexture(OpenGLTexture inTexture);
	void DeleteObject();



protected:
	VAO ObjectVAO;
	OpenGLShader Shader;
	eastl::vector<OpenGLTexture> Textures;
};