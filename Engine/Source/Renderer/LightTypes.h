#pragma once


struct DirLightData
{


};

struct PointLightData
{
	float Linear;
	float Quadratic;

	glm::vec3 Color;
};

enum class ELightType
{
	Directional,
	Point
};

struct LightData
{
	ELightType Type;
	TransformObjPtr Source;

	union {
		DirLightData DirData;
		PointLightData PointData;
	} TypeData;
};
