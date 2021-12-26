#include "RenderableObject.h"


RenderableObject::RenderableObject(VAO& inVAO, OpenGLShader& inShader)
	: ObjectVAO{ inVAO }, Shader{ inShader }
{
}

RenderableObject::~RenderableObject()
{
}

void RenderableObject::Tick(const float inDeltaT)
{

}

void RenderableObject::DeleteObject()
{
	ObjectVAO.DeleteBuffers();
	Shader.DeleteShader();
}
