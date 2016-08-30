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

