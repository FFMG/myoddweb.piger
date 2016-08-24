#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestFloatingPoint* 
TEST_MEM(AnyTestFloatingPoint, CheckTheDataTypeDouble)
{
  double value = RealRandomNumber<double>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_double, x.Type());
}

TEST_MEM(AnyTestFloatingPoint, CheckTheDataTypeFloat)
{
  float value = RealRandomNumber<float>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_float, x.Type());
}

TEST_MEM(AnyTestFloatingPoint, CheckTheDataTypeLongDouble)
{
  long double value = RealRandomNumber<long double>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_long_double, x.Type());
}

TEST_MEM(AnyTestFloatingPoint, DefaultNumberIsZeroDouble)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ( 0, (double)x );
}

TEST_MEM(AnyTestFloatingPoint, DefaultNumberIsZeroFloat)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ(0, (float)x);
}

TEST_MEM(AnyTestFloatingPoint, DefaultNumberIsZeroLongDouble)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ(0, (long double)x);
}

TEST_MEM(AnyTestFloatingPoint, ALongNumberCanBeALongDouble)
{
  long value = IntRandomNumber<long>();
  auto x = myodd::dynamic::Any( value );
  ASSERT_EQ((long double)value, (long double)x);
}

TEST_MEM(AnyTestFloatingPoint, ALongNumberCanBeADouble)
{
  long value = IntRandomNumber<long>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ((double)value, (double)x);
}

TEST_MEM(AnyTestFloatingPoint, AnIntNumberCanBeAFloat)
{
  int value = IntRandomNumber<int>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ((float)value, (float)x);
}