#pragma once
#include "Math/Transform.h"
#include "EASTL/shared_ptr.h"
#include "EASTL/vector.h"

using EntityPtr = eastl::shared_ptr<class Entity>;
using EntityIterator = eastl::vector<eastl::shared_ptr<class Entity>>::iterator;

/**
 * Main Game Entity with Transform and Init and Tick functions
 */
class Entity : public eastl::enable_shared_from_this<Entity>
{
public:
	Entity();
	virtual ~Entity() = default;

	virtual void Init();
	virtual void Tick(const float inDeltaT) = 0;

	inline void AddChild(EntityPtr inEntity) { inEntity->SetParent(shared_from_this()); }
	inline glm::vec3 GetLocation() { return Model.Translation; }
	inline const eastl::vector<EntityPtr> GetChildren() const { return Children; }
	void SetParent(eastl::shared_ptr<Entity> inParent);
	inline eastl::weak_ptr<Entity>& GetParent() { return Parent; }
	
	inline const Transform& GetRelativeTransform() const { return Model; }
	inline Transform& GetRelativeTransform() { return Model; }
	const Transform GetAbsoluteTransform() const;

protected:
	int32_t EntityId;
	Transform Model;
	eastl::weak_ptr<Entity> Parent;

private:
	eastl::vector<EntityPtr> Children;
	static int32_t Entities;

	friend class Scene;
};