 #pragma once
#include "Utils/InlineAllocator.h"
#include "EvenSystem/IFunctionContainerBase.h"
 
class InlineAllocator;

 class DelegateBase
 {

 protected:
	DelegateBase();
	virtual ~DelegateBase();

	DelegateBase(const DelegateBase& inOther);
	DelegateBase& operator=(const DelegateBase& inOther);

	DelegateBase(DelegateBase&& inOther);
	DelegateBase& operator=(DelegateBase&& inOther);

 protected:
	 void Unbind();

	 inline IFunctionContainerBase* GetDelegateInstance() const { return reinterpret_cast<IFunctionContainerBase*>(Allocator.GetAllocation()); }

	 void* Allocate(size_t inSize);

	 friend void* operator new(size_t inSize, DelegateBase& inBase);

 protected:
	 InlineAllocator Allocator;
 };

 inline void* operator new(size_t inSize, DelegateBase& inBase)
 {
	 return inBase.Allocate(inSize);
 }
