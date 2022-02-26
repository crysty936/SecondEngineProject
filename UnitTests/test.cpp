#include "gtest/gtest.h"
#include "EventSystem/EventSystem.h"

namespace DelegatesTests
{
    const int BoundDelegateSize = 24;
    const int FreeFuncDelegateSize = 16;
    const int UnBoundDelegateSize = 0;

	struct DelegatesTestClass
	{
		void SimpleMemberFunction() {}
		
		int ReturnTypeMemberFunction(int inParam1, int inParam2)
		{
			int foo = 2;

			return foo;
		}

        void ConstMemberFunction() const
        {

        }

        static void StaticFunc()
        {

        }

        static int RetStaticFunc(int inParam1, int inParam2)
        {
            return inParam1 + inParam2;
        }
	};

 	TEST(MemberDelegate, MemberDelegateSizeCreateTest)
 	{
 		using testDelegate = Delegate<void>;
 		DelegatesTestClass test;
 
  		testDelegate del = testDelegate::CreateRaw(&test, &DelegatesTestClass::SimpleMemberFunction);
 
 		EXPECT_EQ(del.GetAllocatedSize(), 24);
 	}
 
 	TEST(MemberDelegate, EmptyMemberDelegateCreateSize)
 	{
 		using testDelegate = Delegate<void>;
 
 		testDelegate foo;
 
 		EXPECT_EQ(foo.GetAllocatedSize(), 0);
 	}  
 
 	TEST(MemberDelegate, MemberDelegateRValueConstructorCreateTest)
 	{
 		using testDelegate = Delegate<int, int, int>;
       
 		DelegatesTestClass testClass;
 
 		testDelegate del = testDelegate::CreateRaw(&testClass, &DelegatesTestClass::ReturnTypeMemberFunction);
 		 
        size_t allocatedSize = del.GetAllocatedSize();
        EXPECT_EQ(allocatedSize, BoundDelegateSize);

        testDelegate constructedDelegate = std::move(del);

        allocatedSize = del.GetAllocatedSize();

        EXPECT_EQ(allocatedSize, UnBoundDelegateSize);

        allocatedSize = constructedDelegate.GetAllocatedSize();

		EXPECT_EQ(allocatedSize, BoundDelegateSize);

        int returnValue = constructedDelegate.Execute(2, 3);

        EXPECT_EQ(returnValue, 2);
    }

    TEST(MemberDelegate, ConstMemberDelegateCreateTest)
    {
        using testDelegate = Delegate<void>;

        const DelegatesTestClass testClass;

        testDelegate del = testDelegate::CreateRaw(&testClass, &DelegatesTestClass::ConstMemberFunction);

        EXPECT_EQ(del.GetAllocatedSize(), BoundDelegateSize);
    }

    TEST(MemberDelegate, MemberDelegateBindSizeTest)
    {
        using testDelegate = Delegate<void>;
        
        DelegatesTestClass testClass;

        testDelegate del;

        del.BindRaw(&testClass, &DelegatesTestClass::SimpleMemberFunction);

        size_t delegateSize = del.GetAllocatedSize();

        EXPECT_EQ(delegateSize, BoundDelegateSize);

    }

	TEST(MemberDelegate, ConstMemberDelegateBindSizeTest)
	{
		using testDelegate = Delegate<void>;

		const DelegatesTestClass testClass;

		testDelegate del;

		del.BindRaw(&testClass, &DelegatesTestClass::ConstMemberFunction);

		size_t delegateSize = del.GetAllocatedSize();

		EXPECT_EQ(delegateSize, BoundDelegateSize);
	}

    TEST(MulticastDelegate, BoundMulticastDelegateSize)
    {
        using testMulticastDelegate = MulticastDelegate<>;

        testMulticastDelegate del;

        DelegatesTestClass testClass;

        del.BindRaw(&testClass, &DelegatesTestClass::SimpleMemberFunction);

        del.BindRaw(&testClass, &DelegatesTestClass::SimpleMemberFunction);

        EXPECT_EQ(del.GetAllocationsSize(), BoundDelegateSize * 2);
    }

	TEST(MulticastDelegate, UnBoundMulticastDelegateSize)
	{
		using testMulticastDelegate = MulticastDelegate<>;

		testMulticastDelegate del;

		EXPECT_EQ(del.GetAllocationsSize(), UnBoundDelegateSize);
	}

    TEST(MulticastDelegate, RValueMulticastDelegateTest)
    {
		using testMulticastDelegate = MulticastDelegate<>;

		testMulticastDelegate del;

		DelegatesTestClass testClass;

		del.BindRaw(&testClass, &DelegatesTestClass::SimpleMemberFunction);

		del.BindRaw(&testClass, &DelegatesTestClass::SimpleMemberFunction);

        testMulticastDelegate rValueConstructedDel = std::move(del);

        EXPECT_EQ(del.GetAllocationsSize(), UnBoundDelegateSize);

        EXPECT_EQ(rValueConstructedDel.GetAllocationsSize(), BoundDelegateSize * 2);
    }


    TEST(MulticastDelegate, InvokeMulticastDelegateTest)
    {
        using testMulticastDelegate = MulticastDelegate<>;

        testMulticastDelegate del;

        del.BindStatic(&DelegatesTestClass::StaticFunc);
    }

    TEST(FreeDelegate, BoundFreeDelegateSizeTest)
    {
        using testFreeDelegate = Delegate<void>;

        testFreeDelegate del = testFreeDelegate::CreateStatic(&DelegatesTestClass::StaticFunc);

        size_t delegateSize = del.GetAllocatedSize();

        EXPECT_EQ(delegateSize, FreeFuncDelegateSize);
    }

    TEST(FreeDelegate, RetValueFreeDelegateSizeTest)
    {
        using testFreeDelegate = Delegate<int, int, int>; 

        testFreeDelegate del;

        del.BindStatic(&DelegatesTestClass::RetStaticFunc);

        int result = del.Execute(2, 3);

        EXPECT_EQ(result, 5);
    }



}
