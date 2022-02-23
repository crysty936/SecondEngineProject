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
	virtual ~Entity();

	virtual void Init();
	virtual void Tick(const float inDeltaT);

	inline void AddChild(EntityPtr inEntity) { inEntity->SetParent(shared_from_this()); }
	inline glm::vec3 GetLocation() { return Location; }
	inline const eastl::vector<EntityPtr> GetChildren() const { return Children; }
	void SetParent(eastl::shared_ptr<Entity> inParent);
	inline eastl::weak_ptr<Entity>& GetParent() { return Parent; }
	
	// Transform related
	const Transform GetRelativeTransform() const;
	const Transform& GetAbsoluteTransform() const;
	void CalculateAbsTransform() const;
	void Move(const glm::vec3 inMoveVector);
	void Rotate(const float inAmount, const glm::vec3 inAxis);
	void SetRotation(const glm::vec3 inNewRotation);
	inline void SetScale(const glm::vec3 inScale) { Scale = inScale; }
	inline bool IsTranfsDirty() const { return TransfDirty; }

private:
	void MakeTransfDirty() const;
	inline void CleanTranfsDirty() const { TransfDirty = false; }

private:
	glm::vec3 Location{0.f, 0.f, 0.f};
	glm::vec3 Rotation{0.f, 0.f, 0.f};
	glm::vec3 Scale{1.f, 1.f, 1.f};
	mutable Transform AbsoluteTranfs{};
	mutable bool TransfDirty{ true };
	// Transform related

protected:
	eastl::weak_ptr<Entity> Parent{};
	int32_t EntityId{ ++Entities };

private:
	eastl::vector<EntityPtr> Children{};
	static int32_t Entities;

	friend class Scene;
};