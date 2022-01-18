#include "OpenGLRenderableObject.h"

OpenGLRenderableObject::OpenGLRenderableObject(VAO& inVAO, OpenGLShader& inShader)
	: ObjectVAO{ inVAO }, Shader{ inShader }
{
}

OpenGLRenderableObject::~OpenGLRenderableObject()
{
}

void OpenGLRenderableObject::Tick(const float inDeltaT)
{

}

void OpenGLRenderableObject::Draw()
{
	uint32_t indicesCount = ObjectVAO.GetVertexBuffer().GetIndicesCount();

	eastl::vector<OpenGLTexture>& textures = GetTextures();
	for (OpenGLTexture& tex : textures)
	{
		tex.Bind();
	}

	ObjectVAO.Bind();
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);

	Shader.UnBind();
	ObjectVAO.Unbind();
	for (OpenGLTexture& tex : textures)
	{
		tex.Unbind();
	}
}

void OpenGLRenderableObject::AddTexture(OpenGLTexture inTexture)
{
	Textures.push_back(inTexture);
}

void OpenGLRenderableObject::DeleteObject()
{
	ObjectVAO.DeleteBuffers();
	Shader.DeleteShader();
	for (OpenGLTexture& tex : Textures)
	{
		tex.DeleteTexture();
	}
}

void OpenGLRenderableObject::Init()
{
	
}
