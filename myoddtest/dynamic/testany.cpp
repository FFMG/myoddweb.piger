#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTest* 
TEST(AnyTest, CheckTheDataType)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ(myodd::dynamic::Type::type_null, x.Type());
}

TEST(AnyTest, CanCreateTheValueWithNullPointer)
{
  auto x = myodd::dynamic::Any( nullptr );
  ASSERT_EQ(myodd::dynamic::Type::type_null, x.Type());
}

TEST(AnyTest, AddressOfPointerIsNull)
{
  int* i = nullptr;
  auto x = myodd::dynamic::Any(i);
  ASSERT_EQ(myodd::dynamic::Type::type_null, x.Type());
}

TEST(AnyTest, CanCreateWithAPointerValueInt)
{
  auto value = IntRandomNumber<short>();
  auto x = myodd::dynamic::Any( &value );
  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, x.Type());
  ASSERT_EQ(value, x );
}

TEST(AnyTest, CanCreateWithAPointerValueRealDouble)
{
  auto value = RealRandomNumber<double>();
  auto x = myodd::dynamic::Any(&value);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_double, x.Type());
  ASSERT_EQ(value, x);
}

TEST(AnyTest, CompareTwoAnyThatAreNotTheSameValue)
{
  auto valueX = IntRandomNumber<short>();
  auto valueY = 0;
  while (valueX == valueY)
  {
    valueY = IntRandomNumber<short>();
  }

  auto x = myodd::dynamic::Any(valueX);
  auto y = myodd::dynamic::Any(valueY);

  ASSERT_NE(x, y);
}

TEST(AnyTest, EqualOperatorFromNothing)
{
  auto value = IntRandomNumber<short>();
  auto x = myodd::dynamic::Any( value );
  auto y = x;

  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, y.Type());
  ASSERT_EQ(value, y);
}