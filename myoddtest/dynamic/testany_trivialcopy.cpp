#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestTrivialCopy* 

struct TrivialStruct
{
  int a;
  int b;
};

TEST_MEM(AnyTestTrivialCopy, PassStructureAsToConstructor)
{
  TrivialStruct ts = { 10, 20 };
  ::myodd::dynamic::Any any(ts);
  ASSERT_EQ(::myodd::dynamic::Misc_trivial, any.Type());  
}

TEST_MEM(AnyTestTrivialCopy, AssignmentOperator)
{
  TrivialStruct ts = { 10, 20 };
  ::myodd::dynamic::Any any;
  any = ts;
  ASSERT_EQ(::myodd::dynamic::Misc_trivial, any.Type());
}

TEST_MEM(AnyTestTrivialCopy, CopyConstructor)
{
  TrivialStruct ts = { 10, 20 };
  ::myodd::dynamic::Any any(ts);

  auto any2 = any;
  ASSERT_EQ(::myodd::dynamic::Misc_trivial, any2.Type());
}

TEST_MEM_LOOP(AnyTestTrivialCopy, CheckForMemoryLeaks, NUMBER_OF_TESTS)
{
  ::myodd::dynamic::Any any;
  TrivialStruct ts1 = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  any = ts1;

  TrivialStruct ts2 = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  any = ts2;

  TrivialStruct ts3 = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  any = ts3;
  
  auto any2 = any;
  ASSERT_EQ(::myodd::dynamic::Misc_trivial, any2.Type());

  // comapare the copied value.
  ASSERT_NE(ts1, any2);
  ASSERT_NE(ts2, any2);
  ASSERT_EQ(ts3, any2 );

  // compare the current value
  ASSERT_NE(ts1, any );
  ASSERT_NE(ts2, any );
  ASSERT_EQ(ts3, any );
}

TEST_MEM_LOOP(AnyTestTrivialCopy, GetValue, NUMBER_OF_TESTS)
{
  TrivialStruct ts = { IntRandomNumber<int>(), IntRandomNumber<int>() };
  ::myodd::dynamic::Any any(ts);

  TrivialStruct ts2 = any;
  ASSERT_EQ(ts2.a, ts.a);
  ASSERT_EQ(ts2.b, ts.b);
}

TEST_MEM_LOOP(AnyTestTrivialCopy, GetValueDerivedStruct, NUMBER_OF_TESTS)
{
  TrivialStruct ts = { IntRandomNumber<int>(), IntRandomNumber<int>() };
  ::myodd::dynamic::Any any(ts);

  // create a derived struct
  struct DerivedTrivialStruct : TrivialStruct { };

  DerivedTrivialStruct ts2 = any;
  ASSERT_EQ(ts2.a, ts.a);
  ASSERT_EQ(ts2.b, ts.b);
}

