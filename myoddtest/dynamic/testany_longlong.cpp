#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestLongLong* 
TEST(AnyTestLongLong, NullWillReturnAnLong)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ(0, (long)x);
}

TEST(AnyTestLongLong, DefaultConstructorConstLong)
{
  const long long value = IntRandomNumber<long long>();
  auto x = myodd::dynamic::Any( value );
  ASSERT_EQ((long long)value, (long long)x);
}

TEST(AnyTestLongLong, DefaultConstructorNonConstLong)
{
  long value = IntRandomNumber<long>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ((long long)value, (long long)x);
}

TEST(AnyTestInt, AValidIntToLongLong)
{
  int value = IntRandomNumber<int>();
  auto x = myodd::dynamic::Any( value );
  ASSERT_EQ((long long)value, (long long)x);
}

TEST(AnyTestLongLong, CheckTheDataTypeLongLong)
{
  long long value = IntRandomNumber<long long>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_long_int, x.Type());
}

TEST(AnyTestLongLong, CheckTheDataTypeLongLongInt)
{
  long long int value = IntRandomNumber<long long int>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_long_int, x.Type());
}

TEST(AnyTestLongLong, CheckTheDataTypeSignedLongLong)
{
  signed long long value = IntRandomNumber<signed long long>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_long_int, x.Type());
}

TEST(AnyTestLongLong, CheckTheDataTypeSignedLongLongInt)
{
  signed long long int value = IntRandomNumber<signed long long int>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_long_int, x.Type());
}

TEST(AnyTestLongLong, CheckTheDataTypeUnsignedLongLong )
{
  unsigned long long value = IntRandomNumber<unsigned long long>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_long_int, x.Type());
}

TEST(AnyTestLongLong, CheckTheDataTypeUnsignedLongLongInt)
{
  unsigned long long int value = IntRandomNumber<unsigned long long int>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_long_int, x.Type());
}

