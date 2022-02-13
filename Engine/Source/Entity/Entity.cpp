#include "Entity.h"

int32_t Entity::Entities = 0;

Entity::Entity()
	: EntityId{ ++Entities }, Model{}, Parent(), Children{}
{
	EntityId = ++Entities;
}

void Entity::Init()
{

}

void Entity::SetParent(eastl::shared_ptr<Entity> inParent)
{
	Parent = inParent;

	inParent->Children.push_back(shared_from_this());
}

const Transform Entity::GetAbsoluteTransform() const
{
	if (EntityPtr parentShared = Parent.lock())
	{
		const Transform result = Model * parentShared->GetAbsoluteTransform();

		return result;
	}

	return Model;
}
