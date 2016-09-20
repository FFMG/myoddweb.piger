#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTest* 

TEST(AnyTest, DefaultConstructor)
{
  auto x = myodd::dynamic::Any();
}

TEST(AnyTest, DefaultConstructorInt)
{
  auto value = 10;
  auto x = myodd::dynamic::Any( value );
  ASSERT_EQ(10, (int)x );
}
