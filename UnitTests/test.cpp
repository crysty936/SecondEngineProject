#include "gtest/gtest.h"
#include "pch.h"
#include "EventSystem/EventSystem.h"

namespace DelegatesTests
{
	struct DelegatesTestClass
	{
		void SimpleMemberFunction() {}
	

	};

	TEST(MemberDelegate, MemberDelegateSize)
	{
		using testDelegate = Delegate<void>;
		DelegatesTestClass test;

 		testDelegate del = testDelegate::CreateRaw(&test, &DelegatesTestClass::SimpleMemberFunction);

		EXPECT_EQ(del.GetAllocatedSize(), 24);

		//test
	}

	TEST(MemberDelegate, EmptyMemberDelegateSize)
	{
		using testDelegate = Delegate<void>;

		testDelegate foo;

		EXPECT_EQ(foo.GetAllocatedSize(), 0);
	}





}
