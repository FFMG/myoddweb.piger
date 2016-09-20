#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestLongLong* 
TEST_MEM(AnyTestLongLong, NullWillReturnAnLong)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ(0, (long)x);
}

TEST_MEM(AnyTestLongLong, DefaultConstructorConstLong)
{
  const long long value = IntRandomNumber<long long>();
  auto x = myodd::dynamic::Any( value );
  ASSERT_EQ((long long)value, (long long)x);
}

TEST_MEM(AnyTestLongLong, DefaultConstructorNonConstLong)
{
  long value = IntRandomNumber<long>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ((long long)value, (long long)x);
}

TEST_MEM(AnyTestInt, AValidIntToLongLong)
{
  int value = IntRandomNumber<int>();
  auto x = myodd::dynamic::Any( value );
  ASSERT_EQ((long long)value, (long long)x);
}

TEST_MEM(AnyTestLongLong, CheckTheDataTypeLongLong)
{
  long long value = IntRandomNumber<long long>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_long_int, x.Type());
}

TEST_MEM(AnyTestLongLong, CheckTheDataTypeLongLongInt)
{
  long long int value = IntRandomNumber<long long int>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_long_int, x.Type());
}

TEST_MEM(AnyTestLongLong, CheckTheDataTypeSignedLongLong)
{
  signed long long value = IntRandomNumber<signed long long>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_long_int, x.Type());
}

TEST_MEM(AnyTestLongLong, CheckTheDataTypeSignedLongLongInt)
{
  signed long long int value = IntRandomNumber<signed long long int>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_long_int, x.Type());
}

TEST_MEM(AnyTestLongLong, CheckTheDataTypeUnsignedLongLong )
{
  unsigned long long value = IntRandomNumber<unsigned long long>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_long_int, x.Type());
}

TEST_MEM(AnyTestLongLong, CheckTheDataTypeUnsignedLongLongInt)
{
  unsigned long long int value = IntRandomNumber<unsigned long long int>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_long_int, x.Type());
}

TEST_MEM(AnyTestLongLong, QuickPostAddToUnsignedLongLong)
{
  unsigned long long int value = std::numeric_limits<unsigned long long int>::max();
  auto any = myodd::dynamic::Any(value -1 );
  any++;
  ASSERT_EQ(std::numeric_limits<unsigned long long int>::max(), any);
}

TEST_MEM(AnyTestLongLong, QuickPostAddToLongLong)
{
  long long int value = std::numeric_limits<long long int>::max();
  auto any = myodd::dynamic::Any(value - 1);
  any++;
  ASSERT_EQ(std::numeric_limits<long long int>::max(), any);
}

TEST_MEM(AnyTestLongLong, QuickPreAddToUnsignedLongLong)
{
  unsigned long long int value = std::numeric_limits<unsigned long long int>::max();
  auto any = myodd::dynamic::Any(value - 1);
  ++any;
  ASSERT_EQ(std::numeric_limits<unsigned long long int>::max(), any);
}

TEST_MEM(AnyTestLongLong, QuickPreAddToLongLong)
{
  unsigned long long int value = std::numeric_limits<long long int>::max();
  auto any = myodd::dynamic::Any(value - 1);
  ++any;
  ASSERT_EQ(std::numeric_limits<long long int>::max(), any);
}
