#include "SimpleShapeDrawable.h"

SimpleShapeDrawable::SimpleShapeDrawable(VAO& inVAO, OpenGLShader& inShader)
	: DrawableBase(inVAO, inShader) {}

SimpleShapeDrawable::SimpleShapeDrawable() = default;
SimpleShapeDrawable::~SimpleShapeDrawable() = default;

void SimpleShapeDrawable::Init()
{

}


void SimpleShapeDrawable::AddTexture(OpenGLTexture inTexture)
{
	Textures.push_back(inTexture);
}

void SimpleShapeDrawable::DeleteObject()
{
	ObjectVAO.DeleteBuffers();
	Shader.DeleteShader();
	for (OpenGLTexture& tex : Textures)
	{
		tex.DeleteTexture();
	}
}

void SimpleShapeDrawable::Draw_Private() const
{
	uint32_t indicesCount = ObjectVAO.GetVertexBuffer().GetIndicesCount();

	const eastl::vector<OpenGLTexture>& textures = GetTextures();
	for (const OpenGLTexture& tex : textures)
	{
		tex.Bind();
	}

	ObjectVAO.Bind();
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);

	Shader.UnBind();
	ObjectVAO.Unbind();
	for (const OpenGLTexture& tex : textures)
	{
		tex.Unbind();
	}
}

