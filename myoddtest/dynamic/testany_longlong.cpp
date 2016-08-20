#include "dynamic/any.h"
#include <gtest/gtest.h>

// --gtest_filter=AnyTestLongLong* 
TEST(AnyTestLongLong, NullWillReturnAnLong)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ(0, (long)x);
}

TEST(AnyTestLongLong, CheckTheDataType)
{
  long long value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ( myodd::dynamic::Type::type_longlong, x.Type() );
}

TEST(AnyTestLongLong, DefaultConstructorConstLong)
{
  auto x = myodd::dynamic::Any( (long long)10 );
  ASSERT_EQ(10, (long long)x);
}

TEST(AnyTestLongLong, DefaultConstructorNonConstLong)
{
  long value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(10, (long long)x);
}

TEST(AnyTestInt, AValidIntToLongLong)
{
  auto x = myodd::dynamic::Any((int)10);
  ASSERT_EQ(10, (long long)x);
}
