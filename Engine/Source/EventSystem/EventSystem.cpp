#include "EventSystem/EventSystem.h"



void EventSystemTest::Test(int delimiter)
{
	// 		 	TemplatedFreeDelegate<void, int> freeTemplatedtest{&testFreeFunc};
//  			freeTemplatedtest.Invoke(2);

  	using testDelegate = Delegate<void, int>;
 
    testDelegate del = testDelegate::CreateRaw(this, &EventSystemTest::Function);

    del.Execute(delimiter);

 	//del.BindRaw(this, &EventSystemTest::Function);
// 
// 	int t = 2;
// 	del.Execute(t);
// 
// 
//   	using testMulticast = TemplatedMulticastDelegate<int>;
//   
//    	testMulticast multiDel{};
//    
//    	multiDel.BindRaw(this, &EventSystemTest::Function);
//   
//   	multiDel.Invoke(2);

}

void EventSystemTest::Function(int intA)
{



}

void EventSystemTest::Function2()
{

}

void testFunc3()
{

}
