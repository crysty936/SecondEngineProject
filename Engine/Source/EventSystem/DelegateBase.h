 #pragma once
#include "Utils/InlineAllocator.h"
#include "EventSystem/IFunctionContainerBase.h"
 
class InlineAllocator;

 class DelegateBase
 {

 public:
 	 inline size_t GetAllocatedSize() const { return Allocator.GetAllocatedSize(); }

 protected:
	DelegateBase();
	virtual ~DelegateBase();

	DelegateBase(const DelegateBase& inOther);
	DelegateBase& operator=(const DelegateBase& inOther);

	DelegateBase(DelegateBase&& inOther);
	DelegateBase& operator=(DelegateBase&& inOther);

 protected:
	 void Unbind();

	 inline const IFunctionContainerBase* GetDelegateInstance() const { return reinterpret_cast<const IFunctionContainerBase*>(Allocator.GetAllocation()); }

	 void* Allocate(size_t inSize);

	 friend void* operator new(size_t inSize, DelegateBase& inBase);

 protected:
	 InlineAllocator Allocator;
 };

 inline void* operator new(size_t inSize, DelegateBase& inBase)
 {
	 return inBase.Allocate(inSize);
 }
