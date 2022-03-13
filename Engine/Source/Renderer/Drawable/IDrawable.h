#pragma once

class IDrawable
{
 public:
// 	IDrawable();
	virtual ~IDrawable() = default;

	//inline void SetVisible(const bool inValue) { bIsVisible = inValue; }
	virtual void SetupDrawCommand() = 0;



//private:
	//bool bIsVisible{ true };
};