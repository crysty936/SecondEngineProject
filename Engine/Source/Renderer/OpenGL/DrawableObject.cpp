#include "DrawableObject.h"

DrawableObject::DrawableObject(VAO& inVAO, OpenGLShader& inShader)
	: ObjectVAO{ inVAO }, Shader{ inShader }
{
}

DrawableObject::DrawableObject()
{}

DrawableObject::~DrawableObject()
{}

void DrawableObject::Tick(const float inDeltaT)
{

}

void DrawableObject::Draw()
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

void DrawableObject::AddTexture(OpenGLTexture inTexture)
{
	Textures.push_back(inTexture);
}

void DrawableObject::DeleteObject()
{
	ObjectVAO.DeleteBuffers();
	Shader.DeleteShader();
	for (OpenGLTexture& tex : Textures)
	{
		tex.DeleteTexture();
	}
}

void DrawableObject::Init()
{
	
}
