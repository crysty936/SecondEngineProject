#pragma once


struct DirLightData
{


};

struct PointLightData
{
	float Linear;
	float Quadratic;
};

enum class ELightType
{
	Directional,
	Point
};

struct LightData
{
	ELightType Type;

	glm::vec3 Color;

	union {
		DirLightData DirData;
		PointLightData PointData;
	} TypeData;
};
