#include "dynamic/any.h"
#include <gtest/gtest.h>

// --gtest_filter=AnyTestLong* 
TEST(AnyTestLong, NullWillReturnAnLong)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ(0, (long)x);
}

TEST(AnyTestLong, CheckTheDataType)
{
  long value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ( myodd::dynamic::Type::type_long, x.Type() );
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

TEST(AnyTestInt, AValidIntToLong)
{
  auto x = myodd::dynamic::Any((int)10);
  ASSERT_EQ(10, (long)x);
}
