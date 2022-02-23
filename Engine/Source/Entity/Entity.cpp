#include "Entity.h"

int32_t Entity::Entities = 0;

Entity::Entity() = default;
Entity::~Entity() = default;

void Entity::Init()
{

}

void Entity::Tick(const float inDeltaT)
{
}

void Entity::SetParent(eastl::shared_ptr<Entity> inParent)
{
	Parent = inParent;

	inParent->Children.push_back(shared_from_this());
}

const Transform Entity::GetRelativeTransform() const
{
	return Transform{ Location, Rotation, Scale };
}

const Transform& Entity::GetAbsoluteTransform() const
 {
 	if (TransfDirty)
 	{
        CalculateAbsTransform();
 	}
 	
 	return AbsoluteTranfs;
 }

void Entity::CalculateAbsTransform() const
{
	Transform result = GetRelativeTransform();
	if (EntityPtr parentShared = Parent.lock())
	{
		result = result * parentShared->GetAbsoluteTransform();
	}

	AbsoluteTranfs = result;

	CleanTranfsDirty();
}

void Entity::Move(const glm::vec3 inMoveVector)
{
	Location += inMoveVector;

	MakeTransfDirty();
}

// void Entity::AddRotation(const glm::vec3 inRotation)
// {
// 
// }

void Entity::Rotate(const float inAmount, const glm::vec3 inAxis)
{
	const glm::quat additiveRotation = glm::angleAxis(glm::radians(inAmount), inAxis);
	const glm::quat currentRotation(Rotation);

	const glm::quat newRotation = currentRotation * additiveRotation;
	const glm::vec3 newRotationAxis = glm::eulerAngles(newRotation);

	SetRotation(newRotationAxis);

	MakeTransfDirty();
}

void Entity::SetRotation(const glm::vec3 inNewRotation)
{
	Rotation = inNewRotation; 
	MakeTransfDirty();
}

void Entity::MakeTransfDirty() const
 {
     for (const eastl::shared_ptr<Entity>& child : Children)
     {
         child->MakeTransfDirty();
     }
    
	 TransfDirty = true;
 }
