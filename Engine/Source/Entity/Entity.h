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
	virtual void Tick(const float inDeltaT);

	inline void AddChild(EntityPtr inEntity) { inEntity->SetParent(shared_from_this()); }
	inline glm::vec3 GetLocation() { return RelativeTransf.Translation; }
	inline const eastl::vector<EntityPtr> GetChildren() const { return Children; }
	void SetParent(eastl::shared_ptr<Entity> inParent);
	inline eastl::weak_ptr<Entity>& GetParent() { return Parent; }
	
	inline const Transform& GetRelativeTransform() const { return RelativeTransf; }
	// TODO: Replace with GetAbsoluteTransform inline and CalculateAbsoluteTransform if it's dirty then return a reference 
	const Transform GetAbsoluteTransform() const;
	inline void Move(const glm::vec3 inMoveVector) { RelativeTransf.Translation += inMoveVector; MakeTransfDirty(); }
	inline void Rotate(const float inAmount, const glm::vec3 inAxis) { RelativeTransf.Rotate(inAmount, inAxis); MakeTransfDirty(); }
	inline void SetScale(const glm::vec3 inScale) { RelativeTransf.Scale = inScale; }
	inline bool IsTranfsDirty() const { return TransfDirty; }

private:
	void MakeTransfDirty() const;
	inline void CleanTranfsDirty() const { TransfDirty = false; }

protected:
	int32_t EntityId;
	Transform RelativeTransf;
	mutable Transform AbsoluteTranfs;
	mutable bool TransfDirty;
	eastl::weak_ptr<Entity> Parent;

private:
	eastl::vector<EntityPtr> Children;
	static int32_t Entities;

	friend class Scene;
};