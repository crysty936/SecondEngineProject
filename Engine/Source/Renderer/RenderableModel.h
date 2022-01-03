#pragma once
#include "Core/ITickableObject.h"
#include "OpenGL/Buffer/VAO.h"
#include "OpenGL/OpenGLShader.h"
#include "OpenGL/OpenGLTexture.h"

class RenderableModel : public ITickableObject
{
public:
	RenderableModel(VAO& inVAO, OpenGLShader& inShader);
	virtual ~RenderableModel();

	virtual void Tick(const float inDeltaT) override;
	inline eastl::vector<OpenGLTexture>& GetTextures(){ return Textures; }
	void AddTexture(OpenGLTexture inTexture);
	void DeleteObject();

public:
	VAO ObjectVAO;
	OpenGLShader Shader;
	eastl::vector<OpenGLTexture> Textures;

};