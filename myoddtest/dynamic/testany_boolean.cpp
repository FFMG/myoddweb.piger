#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestBoolean* 
TEST_MEM(AnyTestBoolean, NullWillReturnFalse)
{
  bool value = BoolRandomNumber();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Boolean_bool, x.Type());
}

TEST_MEM(AnyTestBoolean, ValueIsSaved)
{
  bool value = BoolRandomNumber();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(value, (bool)x );
}

TEST_MEM(AnyTestBoolean, BooleanIsANumberInt)
{
  bool value = BoolRandomNumber();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(value ? 1 : 0 , (int)x);
  ASSERT_EQ(value ? 1 : 0, (long)x);
}

TEST_MEM(AnyTestBoolean, BooleanIsANumberReal)
{
  bool value = BoolRandomNumber();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(value ? 1 : 0, (double)x);
  ASSERT_EQ(value ? 1 : 0, (float)x);
  ASSERT_EQ(value ? 1 : 0, (long double)x);
}

TEST_MEM(AnyTestBoolean, RealNumberCanBeABool)
{
  bool value = BoolRandomNumber();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(value ? 1 : 0, (double)x);
  ASSERT_EQ(value ? 1 : 0, (float)x);
  ASSERT_EQ(value ? 1 : 0, (long double)x);
}

TEST_MEM(AnyTestBoolean, ZeroIsFalse)
{
  auto x = myodd::dynamic::Any(0);
  ASSERT_FALSE( (bool)x );
}

TEST_MEM(AnyTestBoolean, NonZeroIsTrue)
{
  auto x = myodd::dynamic::Any(1);
  ASSERT_TRUE( (bool)x);
}

TEST_MEM(AnyTestCharacter, ValueIskept)
{
  auto b = BoolRandomNumber();
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(b, x);
}
