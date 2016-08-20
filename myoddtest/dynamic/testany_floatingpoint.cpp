#include "dynamic/any.h"
#include <gtest/gtest.h>

// --gtest_filter=AnyTestFloatingPoint* 
TEST(AnyTestFloatingPoint, CheckTheDataTypeDouble)
{
  double value = 10.1;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_double, x.Type());
}

TEST(AnyTestFloatingPoint, CheckTheDataTypeFloat)
{
  float value = 10.1f;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_float, x.Type());
}

TEST(AnyTestFloatingPoint, CheckTheDataTypeLongDouble)
{
  long double value = 10.1;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_long_double, x.Type());
}
