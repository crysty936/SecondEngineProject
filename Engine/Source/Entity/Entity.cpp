#include "Entity.h"

int32_t Entity::Entities = 0;

Entity::Entity()
	: EntityId{ ++Entities }, RelativeTransf{}, AbsoluteTranfs{}, TransfDirty{ true }, Parent(), Children{}
{
	EntityId = ++Entities;
}

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

 const Transform Entity::GetAbsoluteTransform() const
 {
 	if (TransfDirty)
 	{
 		Transform result = RelativeTransf;
 		if (EntityPtr parentShared = Parent.lock())
 		{
 			result = result * parentShared->GetAbsoluteTransform();
 		}
 
 		AbsoluteTranfs = result;
        
        CleanTranfsDirty();

 		return result;
 	}
 	
 	return AbsoluteTranfs;
 }

 void Entity::MakeTransfDirty() const
 {
     for (const eastl::shared_ptr<Entity>& child : Children)
     {
         child->MakeTransfDirty();
     }
    
	 TransfDirty = true;
 }
