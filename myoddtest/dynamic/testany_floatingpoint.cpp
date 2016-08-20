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

TEST(AnyTestFloatingPoint, DefaultNumberIsZeroDouble)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ( 0, (double)x );
}

TEST(AnyTestFloatingPoint, DefaultNumberIsZeroFloat)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ(0, (float)x);
}

TEST(AnyTestFloatingPoint, DefaultNumberIsZeroLongDouble)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ(0, (long double)x);
}

TEST(AnyTestFloatingPoint, ALongNumberCanBeALongDouble)
{
  long value = IntRandomNumber<long>();
  auto x = myodd::dynamic::Any( value );
  ASSERT_EQ((long double)value, (long double)x);
}

TEST(AnyTestFloatingPoint, ALongNumberCanBeADouble)
{
  long value = IntRandomNumber<long>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ((double)value, (double)x);
}

TEST(AnyTestFloatingPoint, AnIntNumberCanBeAFloat)
{
  int value = IntRandomNumber<int>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ((float)value, (float)x);
}