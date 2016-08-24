#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestLong* 
TEST_MEM(AnyTestLong, NullWillReturnAnLong)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ(0, (long)x);
}

TEST_MEM(AnyTestLong, DefaultConstructorConstLong)
{
  const long l = IntRandomNumber<long>();
  auto x = myodd::dynamic::Any( l );
  ASSERT_EQ(l, (long)x);
}

TEST_MEM(AnyTestLong, DefaultConstructorNonConstLong)
{
  long value = IntRandomNumber<long>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(value, (long)x);
}

TEST_MEM(AnyTestLong, AValidIntToLong)
{
  const int i = IntRandomNumber<int>();
  auto x = myodd::dynamic::Any( i );
  ASSERT_EQ( (long)i , (long)x);
}

TEST_MEM(AnyTestLong, CheckTheDataTypeLong)
{
  long value = IntRandomNumber<long>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_int, x.Type());
}

TEST_MEM(AnyTestLong, CheckTheDataTypeLongInt)
{
  long int value = IntRandomNumber<long int>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_int, x.Type());
}

TEST_MEM(AnyTestLong, CheckTheDataTypeSignedLong)
{
  signed long value = IntRandomNumber<signed long>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_int, x.Type());
}

TEST_MEM(AnyTestLong, CheckTheDataTypeSignedLongInt)
{
  signed long int value = IntRandomNumber<signed long int>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_int, x.Type());
}

TEST_MEM(AnyTestLong, CheckTheDataTypeUnsignedLong)
{
  unsigned long value = IntRandomNumber<unsigned long>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_int, x.Type());
}

TEST_MEM(AnyTestLong, CheckTheDataTypeUnsignedLongInt)
{
  unsigned long int value = IntRandomNumber<unsigned long int>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_int, x.Type());
}