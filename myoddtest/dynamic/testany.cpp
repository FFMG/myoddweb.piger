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

TEST(AnyTest, AddressOfPointerIsNullButIntTypeIsRespected)
{
  int* i = nullptr;
  auto x = myodd::dynamic::Any(i);
  ASSERT_EQ(myodd::dynamic::Type::Integer_int, x.Type());
}

TEST(AnyTest, AddressOfPointerIsNullButDoubleTypeIsRespected)
{
  double* d = nullptr;
  auto x = myodd::dynamic::Any(d);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_double, x.Type());
}

TEST(AnyTest, AddressOfPointerIsNullButLongDoubleTypeIsRespected)
{
  long double* d = nullptr;
  auto x = myodd::dynamic::Any(d);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_long_double, x.Type());
}

TEST(AnyTest, AddressOfPointerIsNullButLongIntTypeIsRespected)
{
  long int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_int, x.Type());
}

TEST(AnyTest, AddressOfPointerIsNullButLongLongIntTypeIsRespected)
{
  long long int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_long_int, x.Type());
}

TEST(AnyTest, AddressOfPointerIsNullButShortIntTypeIsRespected)
{
  short int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, x.Type());
}

TEST(AnyTest, AddressOfPointerIsNullButUnsignedIntTypeIsRespected)
{
  unsigned int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_int, x.Type());
}

TEST(AnyTest, AddressOfPointerIsNullButUnsignedLongIntTypeIsRespected)
{
  unsigned long int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_int, x.Type());
}

TEST(AnyTest, AddressOfPointerIsNullButUnsignedLongLongIntTypeIsRespected)
{
  unsigned long long int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_long_int, x.Type());
}

TEST(AnyTest, AddressOfPointerIsNullButUnsignedShortIntTypeIsRespected)
{
  unsigned short int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_short_int, x.Type());
}

TEST(AnyTest, AddressOfPointerIsNullButBoolTypeIsRespected)
{
  bool* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Boolean_bool, x.Type());
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