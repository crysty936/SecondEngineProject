#include "RenderableModel.h"


RenderableModel::RenderableModel(VAO& inVAO, OpenGLShader& inShader)
	: ObjectVAO{ inVAO }, Shader{ inShader }
{
}

RenderableModel::~RenderableModel()
{
}

void RenderableModel::Tick(const float inDeltaT)
{

}

void RenderableModel::AddTexture(OpenGLTexture inTexture)
{
	Textures.push_back(inTexture);
}

void RenderableModel::DeleteObject()
{
	ObjectVAO.DeleteBuffers();
	Shader.DeleteShader();
	for (OpenGLTexture& tex : Textures)
	{
		tex.DeleteTexture();
	}
}
