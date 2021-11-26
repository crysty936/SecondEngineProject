#include "Utils/InlineAllocator.h"
#include "Core/EngineUtils.h"
#include <cstring>
#include <string>

InlineAllocator::InlineAllocator() = default;

InlineAllocator::~InlineAllocator()
{
	Free();
}

InlineAllocator::InlineAllocator(const InlineAllocator& inOther)
{
	if (!inOther.HasAllocation()) { return; }

	memcpy(Allocate(inOther.AllocatedSize), inOther.GetAllocation(), inOther.AllocatedSize);
	AllocatedSize = inOther.AllocatedSize;
}

InlineAllocator& InlineAllocator::operator=(const InlineAllocator& inOther)
{
	if (!inOther.HasAllocation()) { return *this; }

	memcpy(Allocate(inOther.AllocatedSize), inOther.GetAllocation(), inOther.AllocatedSize);
	AllocatedSize = inOther.AllocatedSize;

	return *this;
}

InlineAllocator::InlineAllocator(InlineAllocator&& inOther)
	:AllocatedSize{inOther.AllocatedSize}
{
	inOther.AllocatedSize = 0;

	// Tests if the other has heap allocation as we have moved pointers
	const bool onHeap = HasHeapAllocation();
	if (onHeap)
	{
		std::swap(HeapPtr, inOther.HeapPtr);
	}
	else
	{
		memcpy(StackPtr, inOther.StackPtr, AllocatedSize);
	}
}

InlineAllocator& InlineAllocator::operator=(InlineAllocator&& inOther)
{
	Free();

	AllocatedSize = inOther.AllocatedSize;
	inOther.AllocatedSize = 0;

	// Tests if the other has heap allocation as we have moved pointers
	const bool onHeap = HasHeapAllocation();
	if (onHeap)
	{
		std::swap(HeapPtr, inOther.HeapPtr);
	}
	else
	{
		memcpy(StackPtr, inOther.StackPtr, AllocatedSize);
	}

	return *this;
}

void* InlineAllocator::Allocate(const size_t inSize)
{
	const bool sameSize = inSize == AllocatedSize;
	const bool reqHeap = inSize > StackSize;

	if (sameSize)
	{
		// We have the same size and have heap allocation, return the existing heap ptr
		if (reqHeap)
		{
			return HeapPtr;
		}

		return StackPtr;
	}
	else
	{
		const bool onHeap = HasHeapAllocation();

		Free();

		if (reqHeap)
		{
			void* ptr = operator new(inSize);

			return ptr;
		}
		else
		{
			AllocatedSize = inSize;

			return StackPtr;
		}
	}
}

void* InlineAllocator::GetAllocation() const
{
	const bool hasAllocation = HasAllocation();

	if (!hasAllocation) { return nullptr; }

	const bool onHeap = HasHeapAllocation();

	if (onHeap)
	{
		return HeapPtr;
	}

	return  reinterpret_cast<void*>(StackPtr[0]);
}

void InlineAllocator::Free()
{
	const bool hasAllocation = HasAllocation();

	if (!hasAllocation) { return; }

	const bool onHeap = HasHeapAllocation();

	if (onHeap)
	{
		delete HeapPtr;
	}

	AllocatedSize = 0;
}
