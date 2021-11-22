#pragma once
#include <type_traits>
#include "EASTL/vector.h"

template<typename inRetType, typename... inParamTypes>
struct IDelegateInstance
{
	virtual inRetType Execute(inParamTypes&&...) const = 0;
	virtual ~IDelegateInstance() = default;
};

template<typename inRetType, typename... inParamTypes>
class TemplatedFreeDelegate : public IDelegateInstance<inRetType, inParamTypes...>
{
	using FreeFunctionType = inRetType(*)(inParamTypes...);

public:

	inRetType Execute(inParamTypes&&... inParams) const
	{
		return Func(std::forward<inParamTypes>(inParams)...);
	}

	FreeFunctionType Func;
};

template <typename inObjType, typename inRetType, typename... inParamTypes>
struct TMemFunPtrType
{
	using Type = inRetType(inObjType::*)(inParamTypes...);
};

template<typename inObjType, typename inRetType, typename... inParamTypes>
class TemplatedMemberDelegate : public IDelegateInstance<inRetType, inParamTypes...>
{
	using MemberFunctionType = typename TMemFunPtrType<inObjType, inRetType, inParamTypes...>::Type;

public:
	TemplatedMemberDelegate(inObjType* inObj, MemberFunctionType inMemberFunction)
		:Obj(inObj), MemberFunctionPtr(inMemberFunction)
	{

	}

	virtual ~TemplatedMemberDelegate()
		= default;

	inRetType Execute(inParamTypes&&... inParams) const
	{
		return (Obj->*MemberFunctionPtr)(std::forward<inParamTypes>(inParams)...);
	}

private:
	inObjType* Obj;
	MemberFunctionType MemberFunctionPtr;
};


template<typename inRetType, typename... inParamTypes>
class TemplatedDelegate
{
	//static_assert(std::is_integral<inParamTypes...>::value);

	template<typename T>
	using MemberFunc = typename TMemFunPtrType<T, inRetType, inParamTypes...>::Type;

public:
	TemplatedDelegate() = default;
	~TemplatedDelegate()
	{
		delete DelegateInstance;
	}


	template<typename inObjType>
	static IDelegateInstance<inRetType, inParamTypes...>* CreateRaw(inObjType* inObj, MemberFunc<inObjType> inMemberFunction)
	{
		return new TemplatedMemberDelegate<inObjType, inRetType, inParamTypes...>(inObj, inMemberFunction);
	}


	template<typename inObjType>
	void BindRaw(inObjType* inObj, MemberFunc<inObjType> inMemberFunction)
	{
		DelegateInstance = CreateRaw(inObj, inMemberFunction);
	}

	inRetType Execute(inParamTypes... inParams) const
	{
		return DelegateInstance->Execute(std::forward<inParamTypes>(inParams)...);
	}

private:
	IDelegateInstance<inRetType, inParamTypes...>* DelegateInstance = nullptr;
};

template<typename... inParamTypes>
class TemplatedMulticastDelegate
{
	using RetType = void;
	using delegateType = TemplatedDelegate<RetType, inParamTypes...>;

public:
	TemplatedMulticastDelegate() = default;

	~TemplatedMulticastDelegate()
	{
		delete[] Delegates.data();
	}

	template<typename inObjType>
	void BindRaw(inObjType* inObj, typename TMemFunPtrType<inObjType, RetType, inParamTypes...>::Type inMemberFunction)
	{
		IDelegateInstance<void, inParamTypes...>* del = delegateType::CreateRaw(inObj, inMemberFunction);

		Delegates.push_back(del);
	}

	void Invoke(inParamTypes... inParams)
	{
		for (const IDelegateInstance<void, inParamTypes...>* del : Delegates)
		{
			del->Execute(std::forward<inParamTypes>(inParams)...);
		}
	}

private:
	eastl::vector<IDelegateInstance<void, inParamTypes...>*> Delegates{};
};


class EventSystemTest
{

public:
	void Test();
	void Function(int intA);

};