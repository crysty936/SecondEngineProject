#pragma once

class IDrawable
{
 public:
// 	DrawableBase();
	virtual ~IDrawable() = default;

	//inline void SetVisible(const bool inValue) { bIsVisible = inValue; }
	virtual void SetupDrawCommand() = 0;

//private:
	//bool bIsVisible{ true };
};