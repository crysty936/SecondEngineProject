#pragma once
#include "glm/gtc/quaternion.hpp"

/**
 * Transform containing position, orientation and scale data for entities
 * Order of multiplication for parent child nodes is always ChildTransform * ParentTransform
 */
struct Transform
{
	Transform();
	Transform(const Transform& inOther);

	glm::vec3 Translation;
	glm::vec3 Scale;
	glm::quat Rotation;

	glm::mat4 GetMatrix() const;

	void Rotate(const float inAmount, const glm::vec3 inAxis);

	Transform operator*(const Transform& inOther) const;
	Transform& operator=(const Transform& inOther) = default;
	Transform& operator=(Transform&& inOther) = default;
};