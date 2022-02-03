#include "Transform.h"

glm::mat4 Transform::GetModel() const
{
	glm::mat4 model(1.f);

	model = glm::scale(model, Scale);

	// TODO: Find a better operation, maybe from the Camera Class, rather than doing the same one 3 times
	model = glm::rotate(model, glm::radians(Rotation.x), glm::vec3(1.f, 0.f, 0.f));
	model = glm::rotate(model, glm::radians(Rotation.y), glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, glm::radians(Rotation.z), glm::vec3(0.f, 0.f, 1.f));

	model = glm::translate(model, Translation);

	return model;
}

void Transform::Rotate(const float inAmount, const glm::vec3 inAxis)
{
	glm::quat other = glm::angleAxis(glm::radians(inAmount), inAxis);
	NewRotation *= other;
}

Transform Transform::operator*(const Transform& inOther) const
{
	Transform out;

	out.Rotation = inOther.Rotation * this->Rotation;
	out.Scale = this->Scale * inOther.Scale;

	out.Translation = inOther.Rotation * (inOther.Scale * this->Translation) + inOther.Translation;

	return out;
}

