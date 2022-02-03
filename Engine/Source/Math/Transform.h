#pragma once
#include "glm/gtc/quaternion.hpp"

struct Transform
{
	glm::vec3 Translation = glm::vec3(0.f);
	glm::vec3 Rotation = glm::vec3(0.f);
	glm::vec3 Scale = glm::vec3(1.f);
	glm::quat NewRotation = glm::quat(0.f, 0.f, 0.f, 1.f);

	glm::mat4 GetModel() const;

	void Rotate(const float inAmount, const glm::vec3 inAxis);

	Transform operator*(const Transform& inOther) const;
};