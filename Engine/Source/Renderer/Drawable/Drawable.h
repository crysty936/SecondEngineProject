#pragma once
#include "glm/ext/matrix_float4x4.hpp"
#include "Entity/TransformObject.h"

class DrawableObject : public TransformObject
{
public:
	DrawableObject();
	virtual ~DrawableObject();

	virtual glm::mat4 GetModelMatrix() const { return GetAbsoluteTransform().GetMatrix(); }
	inline void SetVisible(const bool inValue) { bIsVisible = inValue; }
	inline bool IsVisible() const { return bIsVisible; }

private:
	bool bIsVisible{ true };
};

// Drawable Container is the one that sets up the Render Commmands for the DrawableObjects
class IDrawableContainer
{
public:
	virtual ~IDrawableContainer() = default;

	virtual void CreateProxy() = 0;
};



