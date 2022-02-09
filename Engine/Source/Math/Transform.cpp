#include "Transform.h"

Transform::Transform()
	: 
	Translation{ glm::vec3(0.f) },
	Scale{1.f},
	Rotation{1.f, 0.f, 0.f, 0.f }
{

}

glm::mat4 Transform::GetModel() const
{
	glm::mat4 model(1.f);

	model = glm::translate(model, Translation);

	model = model * glm::mat4_cast(Rotation);
	
	model = glm::scale(model, Scale);

	return model;
}

void Transform::Rotate(const float inAmount, const glm::vec3 inAxis)
{
	constexpr glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::quat rotation = glm::angleAxis(glm::radians(inAmount), inAxis);
	Rotation *= rotation;
}

Transform Transform::operator*(const Transform& inOther) const
{
	Transform out;

 	out.Translation = inOther.Rotation * (inOther.Scale * this->Translation) + inOther.Translation;
 	out.Rotation = inOther.Rotation * this->Rotation;
 	out.Scale = this->Scale * inOther.Scale;
 

	return out;
}

