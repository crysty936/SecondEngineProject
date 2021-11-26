#pragma once
#include <type_traits>
#include "EASTL/vector.h"
#include "DelegateBase.h"
#include "EvenSystem/IFunctionContainerBase.h"

template<typename inRetType, typename... inParamTypes>
class IFunctionContainer : IFunctionContainerBase
{
public:
	virtual inRetType Execute(inParamTypes&&...) const = 0;
	virtual ~IFunctionContainer() = default;
};

template<typename inRetType, typename... inParamTypes>
class FreeFunctionContainer : public IFunctionContainer<inRetType, inParamTypes...>
{
	using FreeFunctionType = inRetType(*)(inParamTypes...);

public:
	FreeFunctionContainer(FreeFunctionType* inFunc) noexcept
		: Func{ inFunc }
	{

	}

	inRetType Execute(inParamTypes&&... inParams) const
	{
		return Func(std::forward<inParamTypes>(inParams)...);
	}

	FreeFunctionType Func;
};

template <typename inObjType, typename inRetType, typename... inParamTypes>
struct MemberFuncPtrType
{
	using Type = inRetType(inObjType::*)(inParamTypes...);
};

template<typename inObjType, typename inRetType, typename... inParamTypes>
class MemberFuncContainer : public IFunctionContainer<inRetType, inParamTypes...>
{
	using MemberFunctionType = typename MemberFuncPtrType<inObjType, inRetType, inParamTypes...>::Type;

public:
	MemberFuncContainer(inObjType* inObj, MemberFunctionType inMemberFunction) noexcept
		:Obj(inObj), MemberFunctionPtr(inMemberFunction)
	{

	}

	virtual ~MemberFuncContainer()
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
class Delegate : DelegateBase
{
	template<typename T>
	using MemberFunc = typename MemberFuncPtrType<T, inRetType, inParamTypes...>::Type;

	using FuncContainerType = IFunctionContainer<inRetType, inParamTypes...>;

public:
	Delegate() = default;
	~Delegate()
	{
		delete DelegateInstance;
	}


	template<typename inObjType>
	static Delegate CreateRaw(inObjType* inObj, MemberFunc<inObjType> inMemberFunction)
	{
		Delegate del;

		new (del) MemberFuncContainer<inObjType, inRetType, inParamTypes...>(inObj, inMemberFunction);

		return del;
	}


	template<typename inObjType>
	void BindRaw(inObjType* inObj, MemberFunc<inObjType> inMemberFunction)
	{
		DelegateInstance = CreateRaw(inObj, inMemberFunction);
	}

	inRetType Execute(inParamTypes... inParams) const
	{
		FuncContainerType* funcContainer = GetFuncContainer();

		return funcContainer->Execute(std::forward<inParamTypes>(inParams)...);
	}



private:
	FuncContainerType* GetFuncContainer() const
	{
		IFunctionContainerBase* funcContainerBase = GetDelegateInstance();
		FuncContainerType* funcContainer = static_cast<FuncContainerType*>(funcContainerBase);

		return funcContainer;
	}


	IFunctionContainer<inRetType, inParamTypes...>* DelegateInstance = nullptr;
};

template<typename... inParamTypes>
class MulticastDelegate
{
	using RetType = void;
	using delegateType = Delegate<RetType, inParamTypes...>;

public:
	MulticastDelegate() = default;

	~MulticastDelegate()
	{
		delete[] Delegates.data();
	}

	template<typename inObjType>
	void BindRaw(inObjType* inObj, typename MemberFuncPtrType<inObjType, RetType, inParamTypes...>::Type inMemberFunction)
	{
		IFunctionContainer<void, inParamTypes...>* del = delegateType::CreateRaw(inObj, inMemberFunction);

		Delegates.push_back(del);
	}

	void Invoke(inParamTypes... inParams)
	{
		for (const IFunctionContainer<void, inParamTypes...>* del : Delegates)
		{
			del->Execute(std::forward<inParamTypes>(inParams)...);
		}
	}

private:
	eastl::vector<IFunctionContainer<void, inParamTypes...>*> Delegates{};
};


class EventSystemTest
{

public:
	void Test(int delimiter);
	void Function(int intA);
	void Function2();
};

