#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestOperators* 
TEST(AnyTestOperators, LessThan2xAny)
{
  auto x = myodd::dynamic::Any( 10 );
  auto y = myodd::dynamic::Any( 20);
  ASSERT_TRUE( x < y );
}

TEST(AnyTestOperators, LessThanAnyLessOther)
{
  auto x = myodd::dynamic::Any(10);
  ASSERT_TRUE(x < 20 );
}

TEST(AnyTestOperators, LessThanOtherLessThanAny)
{
  auto x = myodd::dynamic::Any(10);
  ASSERT_TRUE(5 < x );
}

TEST(AnyTestOperators, LessThanAnyMoreThanOther)
{
  auto x = myodd::dynamic::Any(50);
  ASSERT_TRUE(x > 20);
}

TEST(AnyTestOperators, LessThanOtherMoreThanAny)
{
  auto x = myodd::dynamic::Any(10);
  ASSERT_TRUE( 20 > x );
}

TEST(AnyTestOperators, MoreThan2xAny)
{
  auto x = myodd::dynamic::Any(50);
  auto y = myodd::dynamic::Any(20);
  ASSERT_TRUE(x > y);
}

TEST(AnyTestOperators, TwoNullValuesAreNotLessThanEachOthers)
{
  auto x = myodd::dynamic::Any( nullptr );
  auto y = myodd::dynamic::Any( nullptr );
  ASSERT_FALSE(x < y);
}

TEST(AnyTestOperators, NullLhsIsLessThanBigNumber)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any( IntRandomNumber<unsigned int>( false ) );
  ASSERT_TRUE(x < y);
  ASSERT_FALSE(x > y);
}

TEST(AnyTestOperators, NullRhsIsLessThanSmallNegNumber)
{
  //  -v1 number ...
  auto i = IntRandomNumber<int>(false);
  auto x = myodd::dynamic::Any( i > 0 ? -1*i : i );
  auto y = myodd::dynamic::Any(nullptr);
  ASSERT_TRUE(x < y);
  ASSERT_FALSE(x > y);
}
