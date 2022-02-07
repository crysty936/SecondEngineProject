#pragma once
#include "glm/gtc/quaternion.hpp"

struct Transform
{
	Transform();

	glm::vec3 Translation;
	glm::vec3 Scale;
	glm::quat Rotation;

	glm::mat4 GetModel() const;

	void Rotate(const float inAmount, const glm::vec3 inAxis);

	Transform operator*(const Transform& inOther) const;
};