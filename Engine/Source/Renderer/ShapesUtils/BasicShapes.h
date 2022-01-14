#pragma once
#include "EASTL/shared_ptr.h"

class BasicShapes
{
public:
	static eastl::shared_ptr<class OpenGLRenderableObject> GetTriangleRenderable();
	static eastl::shared_ptr<class OpenGLRenderableObject> GetSquareRenderable();
};