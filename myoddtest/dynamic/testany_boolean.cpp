#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestBoolean* 
TEST(AnyTestBoolean, NullWillReturnFalse)
{
  bool value = BoolRandomNumber();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Boolean_bool, x.Type());
}

TEST(AnyTestBoolean, ValueIsSaved)
{
  bool value = BoolRandomNumber();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(value, (bool)x );
}

TEST(AnyTestBoolean, BooleanIsANumberInt)
{
  bool value = BoolRandomNumber();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(value ? 1 : 0 , (int)x);
  ASSERT_EQ(value ? 1 : 0, (long)x);
}

TEST(AnyTestBoolean, BooleanIsANumberReal)
{
  bool value = BoolRandomNumber();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(value ? 1 : 0, (double)x);
  ASSERT_EQ(value ? 1 : 0, (float)x);
  ASSERT_EQ(value ? 1 : 0, (long double)x);
}

TEST(AnyTestBoolean, RealNumberCanBeABool)
{
  bool value = BoolRandomNumber();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(value ? 1 : 0, (double)x);
  ASSERT_EQ(value ? 1 : 0, (float)x);
  ASSERT_EQ(value ? 1 : 0, (long double)x);
}

TEST(AnyTestBoolean, ZeroIsFalse)
{
  auto x = myodd::dynamic::Any(0);
  ASSERT_FALSE( (bool)x );
}

TEST(AnyTestBoolean, NonZeroIsTrue)
{
  auto x = myodd::dynamic::Any(1);
  ASSERT_TRUE( (bool)x);
}