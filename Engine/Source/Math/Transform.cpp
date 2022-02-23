#include "Transform.h"

Transform::Transform()
	: 
	Translation{ glm::vec3(0.f) },
	Scale{1.f},
	Rotation{1.f, 0.f, 0.f, 0.f }
{

}

Transform::Transform(const Transform& inOther)
	:
	Translation{ inOther.Translation },
	Scale{ inOther.Scale },
	Rotation{ inOther.Rotation }
{
}

Transform::Transform(const glm::vec3 inLocation, const glm::vec3 inRotation, const glm::vec3 inScale)
	:
	Translation{ inLocation },
	Scale{ inScale },
	Rotation{ inRotation }
{
}

glm::mat4 Transform::GetMatrix() const
{
	glm::mat4 model(1.f);

	model = glm::translate(model, Translation);

	model = model * glm::mat4_cast(Rotation);
	
	model = glm::scale(model, Scale);

	return model;
}

void Transform::Rotate(const float inAmount, const glm::vec3 inAxis)
{
	glm::quat additiveRotation = glm::angleAxis(glm::radians(inAmount), inAxis);
	Rotation = Rotation * additiveRotation;
}

Transform Transform::operator*(const Transform& inOther) const
{
	Transform out;

 	out.Translation = inOther.Rotation * (inOther.Scale * this->Translation) + inOther.Translation;
 	out.Rotation = inOther.Rotation * this->Rotation;
 	out.Scale = this->Scale * inOther.Scale;
 

	return out;
}

