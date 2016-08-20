#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestFloatingPoint* 
TEST(AnyTestFloatingPoint, CheckTheDataTypeDouble)
{
  double value = RealRandomNumber<double>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_double, x.Type());
}

TEST(AnyTestFloatingPoint, CheckTheDataTypeFloat)
{
  float value = RealRandomNumber<float>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_float, x.Type());
}

TEST(AnyTestFloatingPoint, CheckTheDataTypeLongDouble)
{
  long double value = RealRandomNumber<long double>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_long_double, x.Type());
}
