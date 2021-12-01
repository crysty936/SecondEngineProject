#pragma once
#include <type_traits>
#include "EASTL/vector.h"
#include "DelegateBase.h"
#include "EventSystem/IFunctionContainerBase.h"

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
class Delegate : public DelegateBase
{
	template<typename T>
	using MemberFunc = typename MemberFuncPtrType<T, inRetType, inParamTypes...>::Type;

	template<class inObjType>
	using MemberFuncContainerType = MemberFuncContainer<inObjType, inRetType, inParamTypes...>;

	using FuncContainerType = IFunctionContainer<inRetType, inParamTypes...>;

public:
	Delegate() = default;
	~Delegate() = default;

	template<typename inObjType>
	static Delegate CreateRaw(inObjType* inObj, MemberFunc<inObjType> inMemberFunction)
	{
		Delegate del;

		new(del) MemberFuncContainerType<inObjType>(inObj, inMemberFunction);

		return std::move(del);
	}


	template<typename inObjType>
	void BindRaw(inObjType* inObj, MemberFunc<inObjType> inMemberFunction)
	{
		new(this) MemberFuncContainerType<inObjType>(inObj, inMemberFunction);
	}

	inRetType Execute(inParamTypes... inParams) const
	{
		const FuncContainerType* funcContainer = GetFuncContainer();

		return funcContainer->Execute(std::forward<inParamTypes>(inParams)...);
	}

private:
	const FuncContainerType* GetFuncContainer() const
	{
		const IFunctionContainerBase* funcContainerBase = GetDelegateInstance();
		const FuncContainerType* funcContainer = static_cast<const FuncContainerType*>(funcContainerBase);

		return funcContainer;
	}
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

