#include "TransformObject.h"

TransformObject::TransformObject() = default;
TransformObject::~TransformObject() = default;

void TransformObject::SetParent(TransformObjPtr& inParent)
{
	Parent = inParent;

	inParent->Children.push_back(shared_from_this());
}

const Transform TransformObject::GetRelativeTransform() const
{
	Transform newTransform;
	newTransform.Translation = Location;
	newTransform.Rotation = Rotation;
	newTransform.Scale = Scale;

	return newTransform;
}

const Transform& TransformObject::GetAbsoluteTransform() const
{
	if (TransfDirty)
	{
		CalculateAbsTransform();
	}

	return AbsoluteTranfs;
}

void TransformObject::CalculateAbsTransform() const
{
	Transform result = GetRelativeTransform();
	if (const TransformObjPtr parentShared = Parent.lock())
	{
		result = result * parentShared->GetAbsoluteTransform();
	}

	AbsoluteTranfs = result;

	CleanTranfsDirty();
}

void TransformObject::Move(const glm::vec3 inMoveVector)
{
	Location += inMoveVector;

	MakeTransfDirty();
}

void TransformObject::Rotate(const float inAmount, const glm::vec3 inAxis)
{
	const float radians = glm::radians(inAmount);
	const glm::quat additiveRotation = glm::angleAxis(radians, inAxis);
	const glm::quat currentRotation(Rotation);

	const glm::quat newRotation = currentRotation * additiveRotation;
	const glm::vec3 newRotationEuler = glm::eulerAngles(newRotation);

	SetRotation(newRotationEuler);

	glm::mat4 mat = GetAbsoluteTransform().GetMatrix();
}

void TransformObject::SetRotation(const glm::vec3 inNewRotation)
{
	Rotation = inNewRotation;
	MakeTransfDirty();
}

void TransformObject::SetRelTransform(const Transform& inNewTransf)
{
	Location = inNewTransf.Translation;
	Rotation =  glm::eulerAngles(inNewTransf.Rotation);
	Scale = inNewTransf.Scale;
	MakeTransfDirty();
}

void TransformObject::SetRelativeLocation(const glm::vec3 inRelLoc)
{
	Location = inRelLoc;
	MakeTransfDirty();
}

void TransformObject::SetScale(const glm::vec3 inScale)
{
	Scale = inScale;
	MakeTransfDirty();
}

void TransformObject::LookAt(const glm::vec3 inTarget)
{
	const glm::vec3 upVec(0, 1, 0);

	const glm::vec3 z_axis = glm::normalize(inTarget- GetLocation());
	const glm::vec3 x_axis = glm::normalize(glm::cross(upVec, z_axis));
	const glm::vec3 y_axis = glm::normalize(glm::cross(z_axis, x_axis));

	const glm::mat4 rotationMatrix(
		glm::vec4(x_axis, 0),
		glm::vec4(y_axis, 0),
		glm::vec4(z_axis, 0),
		glm::vec4(0, 0, 0, 1));

	const glm::quat q = glm::quat_cast(rotationMatrix);
	const glm::vec3 angles = glm::eulerAngles(q);

	SetRotation(angles);
}

void TransformObject::MakeTransfDirty() const
{
	for (const TransformObjPtr& child : Children)
	{
		child->MakeTransfDirty();
	}

	TransfDirty = true;
}
