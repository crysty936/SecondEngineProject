#pragma once


// Each Drawable type should take care of it's own drawing.
// Even Instanced meshes can be drawn this way by having a parent that inherits from IDrawable contain all of them and not having them as exposed children.
// Culling can be done by the Entity's Transform Data.

class IDrawable
{
public:
	virtual ~IDrawable() = default;

	virtual void Draw() const = 0;
};