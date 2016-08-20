#include "dynamic/any.h"
#include <gtest/gtest.h>

// --gtest_filter=AnyTest* 
TEST(AnyTest, CheckTheDataType)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ(myodd::dynamic::Type::type_null, x.Type());
}
