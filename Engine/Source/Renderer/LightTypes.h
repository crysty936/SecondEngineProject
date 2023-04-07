#pragma once


enum class ELightType
{
	Directional,
	Point
};

struct LightData
{
	ELightType Type;
	TransformObjPtr Source;
};
