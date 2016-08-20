#include "dynamic/any.h"
#include <gtest/gtest.h>

// --gtest_filter=AnyTestInt* 
TEST(AnyTestInt, NullWillReturnAnInt)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ(0, (int)x);
}

TEST(AnyTestInt, CheckTheDataTypeInt)
{
  int value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ( myodd::dynamic::Type::Integer_int, x.Type() );
}

TEST(AnyTestInt, CheckTheDataTypeSigned)
{
  signed value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_int, x.Type());
}

TEST(AnyTestInt, CheckTheDataTypeSignedInt)
{
  signed int value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_int, x.Type());
}

TEST(AnyTestInt, DefaultConstructorConstInt)
{
  auto x = myodd::dynamic::Any( (int)10 );
  ASSERT_EQ(10, (int)x);
}

TEST(AnyTestInt, DefaultConstructorNonConstInt)
{
  int value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(10, (int)x);
}

TEST(AnyTestInt, AValidLongToInt)
{
  auto x = myodd::dynamic::Any((long)10);
  ASSERT_EQ(10, (int)x);
}

TEST(AnyTestInt, CheckTheDataTypeShort )
{
  short value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, x.Type());
}

TEST(AnyTestInt, CheckTheDataTypeShortInt)
{
  short int value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, x.Type());
}

TEST(AnyTestInt, CheckTheDataTypeSignedShort)
{
  signed short value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, x.Type());
}

TEST(AnyTestInt, CheckTheDataTypeSignedShortInt)
{
  signed short int value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, x.Type());
}

TEST(AnyTestInt, CheckTheDataTypeUnsignedShort)
{
  unsigned short value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_short_int, x.Type());
}

TEST(AnyTestInt, CheckTheDataTypeUnsignedShortInt)
{
  unsigned short int value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_short_int, x.Type());
}

TEST(AnyTestInt, CheckTheDataTypeUnsignedUnsigned)
{
  unsigned value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_int, x.Type());
}

TEST(AnyTestInt, CheckTheDataTypeUnsignedUnsignedInt)
{
  unsigned value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_int, x.Type());
}