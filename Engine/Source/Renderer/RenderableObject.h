#pragma once
#include "Core/ITickableObject.h"
#include "OpenGL/Buffer/VAO.h"
#include "OpenGL/OpenGLShader.h"


class RenderableObject : public ITickableObject
{
public:
	RenderableObject(VAO& inVAO, OpenGLShader& inShader);
	virtual ~RenderableObject();

	virtual void Tick(const float inDeltaT) override;
	void DeleteObject();

public:
	VAO ObjectVAO;
	OpenGLShader Shader;
};