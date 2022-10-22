#pragma once
#include "EASTL/shared_ptr.h"
#include "Renderer/Model/3D/Model3D.h"

class MirrorQuad : public DrawableObject, public IDrawableContainer
{
public:
	MirrorQuad();
	virtual ~MirrorQuad();

	void CreateProxy() override;
};