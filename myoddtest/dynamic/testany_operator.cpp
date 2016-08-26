#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestArithmeticConversions* 
TEST_MEM(AnyTestArithmeticConversions, AddTwoLongDoubles )
{
  auto any1 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any2 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any = any1 + any2;
  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
}

TEST_MEM(AnyTestArithmeticConversions, AddLongDoublePlusDouble)
{
  auto any1 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any2 = myodd::dynamic::Any(RealRandomNumber<double>());
  auto any = any1 + any2;
  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
}

TEST_MEM(AnyTestArithmeticConversions, AddLongDoublePlusFloat)
{
  auto any1 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any2 = myodd::dynamic::Any(RealRandomNumber<float>());
  auto any = any1 + any2;
  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
}

TEST_MEM(AnyTestArithmeticConversions, AddLongDoublePlusShortInt)
{
  auto any1 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any2 = myodd::dynamic::Any(IntRandomNumber<short int>());
  auto any = any1 + any2;
  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
}

TEST_MEM(AnyTestArithmeticConversions, AddLongDoublePlusUnsignedShortInt)
{
  auto any1 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any2 = myodd::dynamic::Any(IntRandomNumber<unsigned short int>());
  auto any = any1 + any2;
  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
}

TEST_MEM(AnyTestArithmeticConversions, AddLongDoublePlusInt)
{
  auto any1 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any2 = myodd::dynamic::Any(IntRandomNumber<int>());
  auto any = any1 + any2;
  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
}

TEST_MEM(AnyTestArithmeticConversions, AddLongDoublePlusUnsignedInt)
{
  auto any1 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any2 = myodd::dynamic::Any(IntRandomNumber<unsigned int>());
  auto any = any1 + any2;
  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
}

TEST_MEM(AnyTestArithmeticConversions, AddLongDoublePlusLongInt)
{
  auto any1 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any2 = myodd::dynamic::Any(IntRandomNumber<long int>());
  auto any = any1 + any2;
  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
}

TEST_MEM(AnyTestArithmeticConversions, AddLongDoublePlusUnsignedLongInt)
{
  auto any1 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any2 = myodd::dynamic::Any(IntRandomNumber<unsigned long int>());
  auto any = any1 + any2;
  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
}

TEST_MEM(AnyTestArithmeticConversions, AddLongDoublePlusLongLongInt)
{
  auto any1 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any2 = myodd::dynamic::Any(IntRandomNumber<long long int>());
  auto any = any1 + any2;
  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
}

TEST_MEM(AnyTestArithmeticConversions, AddLongDoublePlusUnsignedLongLongInt)
{
  auto any1 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any2 = myodd::dynamic::Any(IntRandomNumber<unsigned long long int>());
  auto any = any1 + any2;
  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
}
