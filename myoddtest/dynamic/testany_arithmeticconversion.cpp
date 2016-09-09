#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestArithmeticConversions* 
TEST_MEM_LOOP(AnyTestArithmeticConversions, AddTwoLongDoubles, NUMBER_OF_TESTS )
{
  auto any1 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any2 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any = any1 + any2;
  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
}

TEST_MEM_LOOP(AnyTestArithmeticConversions, AddLongDoublesToLongInt, NUMBER_OF_TESTS)
{
  auto any1 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any2 = myodd::dynamic::Any(IntRandomNumber<long int>());
  auto any = any1 + any2;
  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
}

TEST_MEM_LOOP(AnyTestArithmeticConversions, AddLongDoublesToUnsignedLongInt, NUMBER_OF_TESTS)
{
  auto any1 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any2 = myodd::dynamic::Any(IntRandomNumber<unsigned long int>());
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

TEST_MEM_LOOP(AnyTestArithmeticConversions, AddLongDoublePlusUnsignedLongLongInt, NUMBER_OF_TESTS)
{
  auto any1 = myodd::dynamic::Any(RealRandomNumber<long double>());
  auto any2 = myodd::dynamic::Any(IntRandomNumber<unsigned long long int>());
  auto any = any1 + any2;
  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
}

TEST_MEM_LOOP(AnyTestArithmeticConversions, AddTwoLongLongInt, NUMBER_OF_TESTS)
{
  auto x = IntRandomNumber<long long int>();
  auto y = IntRandomNumber<long long int>();

  auto any1 = myodd::dynamic::Any(x);
  auto any2 = myodd::dynamic::Any(y);
  auto any = any1 + any2;
  ASSERT_EQ(myodd::dynamic::Integer_long_long_int , any.Type());
  ASSERT_EQ(x+y, any );
}

TEST_MEM_LOOP(AnyTestArithmeticConversions, AddLongLongIntToUnsignedLongLongInt, NUMBER_OF_TESTS)
{
  auto x = IntRandomNumber<long long int>();
  auto y = IntRandomNumber<unsigned long long int>();

  auto any1 = myodd::dynamic::Any(x);
  auto any2 = myodd::dynamic::Any(y);
  auto any = any1 + any2;

  // signed + unsigned = unsigned
  ASSERT_EQ(myodd::dynamic::Integer_unsigned_long_long_int, any.Type());
  ASSERT_EQ(x + y, any);
}

TEST_MEM_LOOP(AnyTestArithmeticConversions, AddUnsignedLongLongIntToLongLongInt, NUMBER_OF_TESTS)
{
  auto x = IntRandomNumber<unsigned long long int>();
  auto y = IntRandomNumber<long long int>();

  auto any1 = myodd::dynamic::Any(x);
  auto any2 = myodd::dynamic::Any(y);
  auto any = any1 + any2;

  // signed + unsigned = unsigned
  ASSERT_EQ(myodd::dynamic::Integer_unsigned_long_long_int, any.Type());
  ASSERT_EQ(x + y, any);
}

TEST_MEM_LOOP(AnyTestArithmeticConversions, AddLongDoubleToLongLongInt, NUMBER_OF_TESTS)
{
  auto x = RealRandomNumber<long double>();
  auto y = IntRandomNumber<long long int>();

  auto any1 = myodd::dynamic::Any(x);
  auto any2 = myodd::dynamic::Any(y);
  auto any = any1 + any2;

  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
  ASSERT_EQ(x + y, any);
}

TEST_MEM_LOOP(AnyTestArithmeticConversions, AddLongDoubleToUnsignedLongLongInt, NUMBER_OF_TESTS)
{
  auto x = RealRandomNumber<long double>();
  auto y = IntRandomNumber<unsigned long long int>();

  auto any1 = myodd::dynamic::Any(x);
  auto any2 = myodd::dynamic::Any(y);
  auto any = any1 + any2;

  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
  ASSERT_EQ(x + y, any);
}

TEST_MEM_LOOP(AnyTestArithmeticConversions, AddUnsignedLongLongIntToLongDouble, NUMBER_OF_TESTS)
{
  auto x = IntRandomNumber<unsigned long long int>();
  auto y = RealRandomNumber<long double>();

  auto any1 = myodd::dynamic::Any(x);
  auto any2 = myodd::dynamic::Any(y);
  auto any = any1 + any2;

  ASSERT_EQ(myodd::dynamic::Floating_point_long_double, any.Type());
  ASSERT_EQ(x + y, any);
}