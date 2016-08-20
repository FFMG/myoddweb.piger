#include "dynamic/any.h"
#include <gtest/gtest.h>

// --gtest_filter=AnyTestLong* 
TEST(AnyTestLong, NullWillReturnAnLong)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ(0, (long)x);
}

TEST(AnyTestLong, DefaultConstructorConstLong)
{
  auto x = myodd::dynamic::Any( (long)10 );
  ASSERT_EQ(10, (long)x);
}

TEST(AnyTestLong, DefaultConstructorNonConstLong)
{
  long value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(10, (long)x);
}

TEST(AnyTestLong, AValidIntToLong)
{
  auto x = myodd::dynamic::Any((int)10);
  ASSERT_EQ(10, (long)x);
}

TEST(AnyTestLong, CheckTheDataTypeLong)
{
  long value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_int, x.Type());
}

TEST(AnyTestLong, CheckTheDataTypeLongInt)
{
  long int value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_int, x.Type());
}

TEST(AnyTestLong, CheckTheDataTypeSignedLong)
{
  signed long value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_int, x.Type());
}

TEST(AnyTestLong, CheckTheDataTypeSignedLongInt)
{
  signed long int value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_int, x.Type());
}

TEST(AnyTestLong, CheckTheDataTypeUnsignedLong)
{
  unsigned long value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_int, x.Type());
}

TEST(AnyTestLong, CheckTheDataTypeUnsignedLongInt)
{
  unsigned long int value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_int, x.Type());
}