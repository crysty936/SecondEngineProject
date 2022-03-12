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
	return Transform{ Location, Rotation, Scale };
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
	const glm::quat additiveRotation = glm::angleAxis(glm::radians(inAmount), inAxis);
	const glm::quat currentRotation(Rotation);

	const glm::quat newRotation = currentRotation * additiveRotation;
	const glm::vec3 newRotationAxis = glm::eulerAngles(newRotation);

	SetRotation(newRotationAxis);

	MakeTransfDirty();
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

void TransformObject::MakeTransfDirty() const
{
	for (const TransformObjPtr& child : Children)
	{
		child->MakeTransfDirty();
	}

	TransfDirty = true;
}
