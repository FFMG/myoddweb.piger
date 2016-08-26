#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestOperators* 
TEST_MEM(AnyTestOperators, LessThan2xAny)
{
  auto x = myodd::dynamic::Any( 10 );
  auto y = myodd::dynamic::Any( 20);
  ASSERT_TRUE( x < y );
}

TEST_MEM(AnyTestOperators, LessThanAnyLessOther)
{
  auto x = myodd::dynamic::Any(10);
  ASSERT_TRUE(x < 20 );
}

TEST_MEM(AnyTestOperators, LessThanOtherLessThanAny)
{
  auto x = myodd::dynamic::Any(10);
  ASSERT_TRUE(5 < x );
}

TEST_MEM(AnyTestOperators, LessThanAnyMoreThanOther)
{
  auto x = myodd::dynamic::Any(50);
  ASSERT_TRUE(x > 20);
}

TEST_MEM(AnyTestOperators, LessThanOtherMoreThanAny)
{
  auto x = myodd::dynamic::Any(10);
  ASSERT_TRUE( 20 > x );
}

TEST_MEM(AnyTestOperators, MoreThan2xAny)
{
  auto x = myodd::dynamic::Any(50);
  auto y = myodd::dynamic::Any(20);
  ASSERT_TRUE(x > y);
}

TEST_MEM(AnyTestOperators, TwoNullValuesAreNotLessThanEachOthers)
{
  auto x = myodd::dynamic::Any( nullptr );
  auto y = myodd::dynamic::Any( nullptr );
  ASSERT_FALSE(x < y);
  ASSERT_TRUE(x == y);
}

TEST_MEM(AnyTestOperators, TwoNullValuesAreNotMoreThanEachOthers)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(nullptr);
  ASSERT_FALSE(x > y);
  ASSERT_TRUE(x == y);
}

TEST_MEM(AnyTestOperators, TwoNullValuesCanBeLessOrEqual)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(nullptr);
  ASSERT_TRUE(x <= y);
  ASSERT_TRUE(x == y);
}

TEST_MEM(AnyTestOperators, TwoNullValuesCanBeMoreOrEqual)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(nullptr);
  ASSERT_TRUE(x >= y);
  ASSERT_TRUE(x == y);
}

TEST_MEM(AnyTestOperators, NullLhsIsLessThanBigNumber)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any( IntRandomNumber<unsigned int>( false ) );
  ASSERT_TRUE(x < y);
  ASSERT_FALSE(x > y);
  ASSERT_FALSE(x >= y);
}

TEST_MEM(AnyTestOperators, NullRhsIsLessThanSmallNegNumber)
{
  //  -v1 number ...
  auto i = IntRandomNumber<int>(false);
  auto x = myodd::dynamic::Any( i > 0 ? -1*i : i );
  auto y = myodd::dynamic::Any(nullptr);
  ASSERT_TRUE(x < y);
  ASSERT_FALSE(x > y);
  ASSERT_FALSE(x >= y);
}

TEST_MEM(AnyTestOperators, NullEqualsAnyNul )
{
  auto x = myodd::dynamic::Any(nullptr);
  ASSERT_TRUE( x == nullptr );
  ASSERT_TRUE(nullptr ==  x);
}

TEST_MEM(AnyTestOperators, NullPlusNullEqualZero)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(nullptr);
  auto z = x + y;
  ASSERT_EQ( 0, z);
  ASSERT_EQ( myodd::dynamic::Integer_int, z.Type() );
  ASSERT_EQ(nullptr, x);
  ASSERT_EQ(myodd::dynamic::Misc_null, x.Type());
  ASSERT_EQ(nullptr, y);
  ASSERT_EQ(myodd::dynamic::Misc_null, y.Type());
}

TEST_MEM(AnyTestOperators, NullMinusNullEqualZero)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(nullptr);
  auto z = x - y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(myodd::dynamic::Integer_int, z.Type());
  ASSERT_EQ(nullptr, x);
  ASSERT_EQ(myodd::dynamic::Misc_null, x.Type());
  ASSERT_EQ(nullptr, y);
  ASSERT_EQ(myodd::dynamic::Misc_null, y.Type());
}

TEST_MEM(AnyTestOperators, NullPlusNullEqualZeroShortcut)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(nullptr);
  x += y;

  // x is updated
  ASSERT_EQ(0, x);
  ASSERT_EQ(myodd::dynamic::Integer_int, x.Type());

  // y has not changed
  ASSERT_EQ(nullptr, y);
  ASSERT_EQ(myodd::dynamic::Misc_null, y.Type());
}

TEST_MEM(AnyTestOperators, NullMinusNullEqualZeroShortcut)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(nullptr);
  x -= y;

  // x is updated
  ASSERT_EQ(0, x);
  ASSERT_EQ(myodd::dynamic::Integer_int, x.Type());

  // y has not changed
  ASSERT_EQ(nullptr, y);
  ASSERT_EQ(myodd::dynamic::Misc_null, y.Type());
}

TEST_MEM(AnyTestOperators, AddNumberToNull)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(10);
  x += y;
  ASSERT_EQ(10, x);
}

TEST_MEM(AnyTestOperators, SubtractNumberFromNull)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(10);
  x -= y;
  ASSERT_EQ(-10, x);
}

TEST_MEM(AnyTestOperators, MultiplyNumberAndNull)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(IntRandomNumber<int>(false));
  x *= y;
  ASSERT_EQ(0, x);
}

TEST_MEM(AnyTestOperators, AddNullToNumber)
{
  auto x = myodd::dynamic::Any(10);
  auto y = myodd::dynamic::Any(nullptr);
  x += y;
  ASSERT_EQ(10, x);
}

TEST_MEM(AnyTestOperators, SubstractNullFromNumber)
{
  auto x = myodd::dynamic::Any(10);
  auto y = myodd::dynamic::Any(nullptr);
  x -= y;
  ASSERT_EQ(10, x);
}

TEST_MEM(AnyTestOperators, MultiplyNullFromNumber)
{
  auto x = myodd::dynamic::Any(IntRandomNumber<int>(false));
  auto y = myodd::dynamic::Any(nullptr);
  x *= y;
  ASSERT_EQ(0, x);
}

TEST_MEM(AnyTestOperators, AddTwoAny)
{
  auto x = myodd::dynamic::Any(10);
  auto y = myodd::dynamic::Any(20);
  x += y;
  ASSERT_EQ(30, x);
}

TEST_MEM(AnyTestOperators, SubstractTwoAny)
{
  auto x = myodd::dynamic::Any(40);
  auto y = myodd::dynamic::Any(10);
  x -= y;
  ASSERT_EQ(30, x);
}

TEST_MEM(AnyTestOperators, MultiplyTwoAny)
{
  auto x = myodd::dynamic::Any(40);
  auto y = myodd::dynamic::Any(10);
  x *= y;
  ASSERT_EQ(400, x);
}

TEST_MEM(AnyTestOperators, AddANumberToAny)
{
  auto x = myodd::dynamic::Any(10);
  x += 20;
  ASSERT_EQ(30, x);
}

TEST_MEM(AnyTestOperators, MultiplyANumberToAny)
{
  auto x = myodd::dynamic::Any(40);
  x -= 10;
  ASSERT_EQ(30, x);
}

TEST_MEM(AnyTestOperators, SubstractANumberToAny)
{
  auto x = myodd::dynamic::Any(40);
  x *= 10;
  ASSERT_EQ(400, x);
}

TEST_MEM(AnyTestOperators, AddAnyToANumber)
{
  auto x = myodd::dynamic::Any(10);
  x = 20 + x;
  ASSERT_EQ(30, x);
}

TEST_MEM(AnyTestOperators, SubstractAnyToANumber)
{
  auto x = myodd::dynamic::Any(10);
  x = 40 - x;
  ASSERT_EQ(30, x);
}

TEST_MEM(AnyTestOperators, MultiplyAnyToANumber)
{
  auto x = myodd::dynamic::Any(10);
  x = 40 * x;
  ASSERT_EQ(400, x);
}

TEST_MEM(AnyTestOperators, AddAnyToANumberAndCreate)
{
  auto x = myodd::dynamic::Any(10);
  auto y = 20 + x;
  ASSERT_EQ(30, y);
}

TEST_MEM(AnyTestOperators, SubstractAnyToANumberAndCreate)
{
  auto x = myodd::dynamic::Any(10);
  auto y = 40 - x;
  ASSERT_EQ(30, y);
}

TEST_MEM(AnyTestOperators, MultiplyAnyToANumberAndCreate)
{
  auto x = myodd::dynamic::Any(10);
  auto y = 40 * x;
  ASSERT_EQ(400, y);
}

TEST_MEM(AnyTestOperators, AddTwoDifferentTypes )
{
  auto x = myodd::dynamic::Any( (int)10 );
  auto y = myodd::dynamic::Any( (double)20.5 );
  auto z = x + y;
  ASSERT_EQ(30.5, z );
  ASSERT_EQ(10, x);
  ASSERT_EQ(20.5, y);
}

TEST_MEM(AnyTestOperators, SubstractwoDifferentTypes)
{
  auto x = myodd::dynamic::Any((int)30);
  auto y = myodd::dynamic::Any((double)10.5);
  auto z = x - y;
  ASSERT_EQ(19.5, z);
  ASSERT_EQ(30, x);
  ASSERT_EQ(10.5, y);
}

TEST_MEM(AnyTestOperators, MultiplytwoDifferentTypes)
{
  auto x = myodd::dynamic::Any((int)30);
  auto y = myodd::dynamic::Any((double)10.5);
  auto z = x * y;
  ASSERT_EQ((double)315, z);// we multiplied a double * int so z is a double.
  ASSERT_EQ(30, x);
  ASSERT_EQ(10.5, y);
}

TEST_MEM(AnyTestOperators, AddTwoBlooean)
{
  auto x = myodd::dynamic::Any( true );
  auto y = myodd::dynamic::Any( false );
  auto z = x + y;
  ASSERT_EQ(1, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(false, y);
}

TEST_MEM(AnyTestOperators, SubstractTwoBlooean)
{
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(false);
  auto z = x - y;
  ASSERT_EQ(1, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(false, y);

  z = y - x;
  ASSERT_EQ(-1, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(false, y);
}

TEST_MEM(AnyTestOperators, MultiplyTwoBlooean)
{
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(false);
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(false, y);

  z = y * x;
  ASSERT_EQ(0, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(false, y);
}

TEST_MEM(AnyTestOperators, MultiplyTwoTrueBlooean)
{
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(true);
  auto z = x * y;
  ASSERT_EQ(1, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(true, y);
}

TEST_MEM(AnyTestOperators, AddANumberToABoolean)
{
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(10);
  auto z = x + y;
  ASSERT_EQ(11, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(10, y);
}

TEST_MEM(AnyTestOperators, SubstractANumberFromABoolean)
{
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(10);
  auto z = x - y;
  ASSERT_EQ(-9, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(10, y);
}

TEST_MEM(AnyTestOperators, MultiplyANumberAndABoolean)
{
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(10);
  auto z = x * y;
  ASSERT_EQ(10, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(10, y);
}

TEST_MEM(AnyTestOperators, AddNullToABooleanBecomesAnInt)
{
  auto x = myodd::dynamic::Any( nullptr );
  auto y = myodd::dynamic::Any( true );
  auto z = x + y;
  ASSERT_EQ(1, z);
  ASSERT_EQ(nullptr, x);
  ASSERT_EQ(true, y);
}

TEST_MEM(AnyTestOperators, SubstractNullFromABooleanBecomesAnInt)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(true);
  auto z = x - y;
  ASSERT_EQ(-1, z);
  ASSERT_EQ(nullptr, x);
  ASSERT_EQ(true, y);
}

TEST_MEM(AnyTestOperators, MultiplyNullAndABooleanBecomesAnInt)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(true);
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(nullptr, x);
  ASSERT_EQ(true, y);
}

TEST_MEM(AnyTestOperators, AddBooleanToNullBecomesAnInt)
{
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(nullptr);
  auto z = x + y;
  ASSERT_EQ(1, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(nullptr, y);
}

TEST_MEM(AnyTestOperators, AddNullToBooleanBecomesAnInt)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(true);
  auto z = x + y;
  ASSERT_EQ(1, z);
  ASSERT_EQ(nullptr, x);
  ASSERT_EQ(true, y);
}

TEST_MEM(AnyTestOperators, SubstractBooleanFromNullBecomesAnInt)
{
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(nullptr);
  auto z = x - y;
  ASSERT_EQ(1, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(nullptr, y);
}

TEST_MEM(AnyTestOperators, MultiplyBooleanAndNullBecomesAnInt)
{
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(nullptr);
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(nullptr, y);
}

TEST_MEM(AnyTestOperators, ChainNumberAddition)
{
  auto w = myodd::dynamic::Any((long)10);
  auto x = myodd::dynamic::Any((long)20);
  auto y = myodd::dynamic::Any((long)30);
  
  auto z = w + x + y;
  ASSERT_EQ((long)60, z);
  ASSERT_EQ((long)10, w);
  ASSERT_EQ((long)20, x);
  ASSERT_EQ((long)30, y);
}

TEST_MEM(AnyTestOperators, ChainNumberSubstraction)
{
  auto w = myodd::dynamic::Any((long)40);
  auto x = myodd::dynamic::Any((long)20);
  auto y = myodd::dynamic::Any((long)10);

  auto z = w - x - y;
  ASSERT_EQ((long)10, z);
  ASSERT_EQ((long)40, w);
  ASSERT_EQ((long)20, x);
  ASSERT_EQ((long)10, y);
}

TEST_MEM(AnyTestOperators, ChainNumberMultiplication)
{
  auto w = myodd::dynamic::Any((long)40);
  auto x = myodd::dynamic::Any((long)20);
  auto y = myodd::dynamic::Any((long)10);

  auto z = w * x * y;
  ASSERT_EQ((long)8000, z);
  ASSERT_EQ((long)40, w);
  ASSERT_EQ((long)20, x);
  ASSERT_EQ((long)10, y);
}

TEST_MEM(AnyTestOperators, AddTwoCharOnlyIfTheCharIsANumber)
{
  auto x = myodd::dynamic::Any('1');
  auto y = myodd::dynamic::Any('9' );
  auto z = x + y;
  ASSERT_EQ(10, z);
  ASSERT_EQ('1', x);
  ASSERT_EQ('9', y);
}

TEST_MEM(AnyTestOperators, SubstractTwoCharOnlyIfTheCharIsANumber)
{
  auto x = myodd::dynamic::Any('9');
  auto y = myodd::dynamic::Any('1');
  auto z = x - y;
  ASSERT_EQ( 8, z);
  ASSERT_EQ('9', x);
  ASSERT_EQ('1', y);
}

TEST_MEM(AnyTestOperators, MultiplyTwoCharOnlyIfTheCharIsANumber)
{
  auto x = myodd::dynamic::Any('9');
  auto y = myodd::dynamic::Any('2');
  auto z = x * y;
  ASSERT_EQ(18, z);
  ASSERT_EQ('9', x);
  ASSERT_EQ('2', y);
}

TEST_MEM(AnyTestOperators, AddTwoWideCharOnlyIfTheCharIsANumber)
{
  auto x = myodd::dynamic::Any(L'1');
  auto y = myodd::dynamic::Any(L'9');
  auto z = x + y;
  ASSERT_EQ(10, z);
  ASSERT_EQ(L'1', x);
  ASSERT_EQ(L'9', y);
}

TEST_MEM(AnyTestOperators, SubstractTwoWideCharOnlyIfTheCharIsANumber)
{
  auto x = myodd::dynamic::Any(L'9');
  auto y = myodd::dynamic::Any(L'1');
  auto z = x - y;
  ASSERT_EQ( 8, z);
  ASSERT_EQ(L'9', x);
  ASSERT_EQ(L'1', y);
}

TEST_MEM(AnyTestOperators, MultiplyTwoWideCharOnlyIfTheCharIsANumber)
{
  auto x = myodd::dynamic::Any(L'9');
  auto y = myodd::dynamic::Any(L'2');
  auto z = x * y;
  ASSERT_EQ(18, z);
  ASSERT_EQ(L'9', x);
  ASSERT_EQ(L'2', y);
}

TEST_MEM(AnyTestOperators, AddTwoCharZeroIfNoneAreNumbers)
{
  auto x = myodd::dynamic::Any('a');
  auto y = myodd::dynamic::Any('&');
  auto z = x + y;
  ASSERT_EQ(0, z);
  ASSERT_EQ('a', x);
  ASSERT_EQ('&', y);
}

TEST_MEM(AnyTestOperators, SubstractTwoCharZeroIfNoneAreNumbers)
{
  auto x = myodd::dynamic::Any('a');
  auto y = myodd::dynamic::Any('&');
  auto z = x - y;
  ASSERT_EQ(0, z);
  ASSERT_EQ('a', x);
  ASSERT_EQ('&', y);
}

TEST_MEM(AnyTestOperators, MultiplyTwoCharZeroIfNoneAreNumbers)
{
  auto x = myodd::dynamic::Any('a');
  auto y = myodd::dynamic::Any('&');
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ('a', x);
  ASSERT_EQ('&', y);
}

TEST_MEM(AnyTestOperators, AddTwoWideCharZeroIfNoneAreNumbers)
{
  auto x = myodd::dynamic::Any(L'a');
  auto y = myodd::dynamic::Any(L'&');
  auto z = x + y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(L'a', x);
  ASSERT_EQ(L'&', y);
}

TEST_MEM(AnyTestOperators, SubstractTwoWideCharZeroIfNoneAreNumbers)
{
  auto x = myodd::dynamic::Any(L'a');
  auto y = myodd::dynamic::Any(L'&');
  auto z = x - y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(L'a', x);
  ASSERT_EQ(L'&', y);
}

TEST_MEM(AnyTestOperators, MultiplyTwoWideCharZeroIfNoneAreNumbers)
{
  auto x = myodd::dynamic::Any(L'a');
  auto y = myodd::dynamic::Any(L'&');
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(L'a', x);
  ASSERT_EQ(L'&', y);
}

TEST_MEM(AnyTestOperators, AddTwoCharIfOneIsANumber)
{
  auto x = myodd::dynamic::Any('1');
  auto y = myodd::dynamic::Any('&');
  auto z = x + y;
  ASSERT_EQ(1, z);
  ASSERT_EQ('1', x);
  ASSERT_EQ('&', y);
}

TEST_MEM(AnyTestOperators, AddTwoStringIfOneIsANumber)
{
  auto x = myodd::dynamic::Any("1234");
  auto y = myodd::dynamic::Any("Hello");
  auto z = x + y;
  ASSERT_EQ(1234, z);
  ASSERT_EQ("1234", x);
  ASSERT_EQ("Hello", y);
}

TEST_MEM(AnyTestOperators, SubtractTwoCharIfOneIsANumber)
{
  auto x = myodd::dynamic::Any('1');
  auto y = myodd::dynamic::Any('&');
  auto z = x - y;
  ASSERT_EQ(1, z);
  ASSERT_EQ('1', x);
  ASSERT_EQ('&', y);
}

TEST_MEM(AnyTestOperators, SubstractTwoStringIfOneIsANumber)
{
  auto x = myodd::dynamic::Any("1234");
  auto y = myodd::dynamic::Any("Hello");
  auto z = x - y;
  ASSERT_EQ(1234, z);
  ASSERT_EQ("1234", x);
  ASSERT_EQ("Hello", y);
}

TEST_MEM(AnyTestOperators, MultiplyTwoCharIfOneIsANumber)
{
  auto x = myodd::dynamic::Any('1');
  auto y = myodd::dynamic::Any('&');
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ('1', x);
  ASSERT_EQ('&', y);
}

TEST_MEM(AnyTestOperators, MultiplyTwoStringIfOneIsANumber)
{
  auto x = myodd::dynamic::Any("1234");
  auto y = myodd::dynamic::Any("Hello");
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ("1234", x);
  ASSERT_EQ("Hello", y);
}

TEST_MEM(AnyTestOperators, AddTwoWideCharIfOneIsANumber)
{
  auto x = myodd::dynamic::Any(L'1');
  auto y = myodd::dynamic::Any(L'&');
  auto z = x + y;
  ASSERT_EQ(1, z);
  ASSERT_EQ(L'1', x);
  ASSERT_EQ(L'&', y);
}

TEST_MEM(AnyTestOperators, AddTwoWideStringIfOneIsANumber)
{
  auto x = myodd::dynamic::Any(L"1234");
  auto y = myodd::dynamic::Any(L"Hello");
  auto z = x + y;
  ASSERT_EQ(1234, z);
  ASSERT_EQ(L"1234", x);
  ASSERT_EQ(L"Hello", y);
}

TEST_MEM(AnyTestOperators, SubstractTwoWideStringIfOneIsANumber)
{
  auto x = myodd::dynamic::Any(L"1234");
  auto y = myodd::dynamic::Any(L"Hello");
  auto z = x - y;
  ASSERT_EQ(1234, z);
  ASSERT_EQ(L"1234", x);
  ASSERT_EQ(L"Hello", y);
}

TEST_MEM(AnyTestOperators, SubstractTwoWideCharIfOneIsANumber)
{
  auto x = myodd::dynamic::Any(L'1');
  auto y = myodd::dynamic::Any(L'&');
  auto z = x - y;
  ASSERT_EQ(1, z);
  ASSERT_EQ(L'1', x);
  ASSERT_EQ(L'&', y);
}

TEST_MEM(AnyTestOperators, MultiplyTwoWideStringIfOneIsANumber)
{
  auto x = myodd::dynamic::Any(L"1234");
  auto y = myodd::dynamic::Any(L"Hello");
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(L"1234", x);
  ASSERT_EQ(L"Hello", y);
}

TEST_MEM(AnyTestOperators, MultiplyTwoWideCharIfOneIsANumber)
{
  auto x = myodd::dynamic::Any(L'1');
  auto y = myodd::dynamic::Any(L'&');
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(L'1', x);
  ASSERT_EQ(L'&', y);
}

TEST_MEM(AnyTestOperators, AddACharAndAWideChar )
{
  auto x = myodd::dynamic::Any(L'1');
  auto y = myodd::dynamic::Any('2');
  auto z = x + y;
  ASSERT_EQ(3, z);
  ASSERT_EQ(L'1', x);
  ASSERT_EQ('2', y);
}

TEST_MEM(AnyTestOperators, SubstractACharAndAWideChar)
{
  auto x = myodd::dynamic::Any(L'4');
  auto y = myodd::dynamic::Any('1');
  auto z = x - y;
  ASSERT_EQ(3, z);
  ASSERT_EQ(L'4', x);
  ASSERT_EQ('1', y);
}

TEST_MEM(AnyTestOperators, MultiplyACharAndAWideChar)
{
  auto x = myodd::dynamic::Any(L'4');
  auto y = myodd::dynamic::Any('2');
  auto z = x * y;
  ASSERT_EQ(8, z);
  ASSERT_EQ(L'4', x);
  ASSERT_EQ('2', y);
}

TEST_MEM(AnyTestOperators, AdditionAndSubstraction)
{
  auto w = myodd::dynamic::Any((long)40);
  auto x = myodd::dynamic::Any((long)20);
  auto y = myodd::dynamic::Any((long)10);

  auto z = w + x - y;
  ASSERT_EQ((long)50, z);
  ASSERT_EQ((long)40, w);
  ASSERT_EQ((long)20, x);
  ASSERT_EQ((long)10, y);
}

TEST_MEM(AnyTestOperators, AdditionAndSubstractionReUseNumbers)
{
  auto w = myodd::dynamic::Any((long)40);
  auto x = myodd::dynamic::Any((long)20);
  auto y = myodd::dynamic::Any((long)10);

  auto z = w + x - y - w + x;
       // 40 + 20 - 10 - 40 + 20 = 30
  ASSERT_EQ((long)30, z);
  ASSERT_EQ((long)40, w);
  ASSERT_EQ((long)20, x);
  ASSERT_EQ((long)10, y);
}

TEST_MEM(AnyTestOperators, MultiplicationAdditionAndSubstractionReUseNumbers)
{
  auto v = myodd::dynamic::Any((long)5);
  auto w = myodd::dynamic::Any((long)40);
  auto x = myodd::dynamic::Any((long)20);
  auto y = myodd::dynamic::Any((long)10);

  auto z = w + x - y - w + x * v;
  // 40 + 20 - 10 - 40 + 20 * 5= 110
  ASSERT_EQ((long)110, z);
  ASSERT_EQ((long)5, v);
  ASSERT_EQ((long)40, w);
  ASSERT_EQ((long)20, x);
  ASSERT_EQ((long)10, y);
}

TEST_MEM(AnyTestOperators, MultiplicationAdditionAndSubstractionParentheseReUseNumbers)
{
  auto v = myodd::dynamic::Any((long)5);
  auto w = myodd::dynamic::Any((long)40);
  auto x = myodd::dynamic::Any((long)20);
  auto y = myodd::dynamic::Any((long)10);

  auto z = w + x - (y - (w + x) * v);
  long zz = 40 + 20 - (10 - (40 + 20) * 5);
  // 40 + 20 - (10 - (40 + 20) * 5)= 350
  ASSERT_EQ(zz, z);
  ASSERT_EQ((long)5, v);
  ASSERT_EQ((long)40, w);
  ASSERT_EQ((long)20, x);
  ASSERT_EQ((long)10, y);
}

TEST_MEM(AnyTestOperators, DivideANumberByNullWillThrow)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>());
  EXPECT_THROW(any / nullptr, std::overflow_error);
}

TEST_MEM(AnyTestOperators, DivideZeroByNullWillThrow)
{
  auto any = myodd::dynamic::Any(0);
  EXPECT_THROW(any / nullptr, std::overflow_error);
}

TEST_MEM(AnyTestOperators, DivideANumberByNullAnyWillThrow)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto nullany = myodd::dynamic::Any( nullptr );
  EXPECT_THROW(any / nullany, std::overflow_error);
}

TEST_MEM(AnyTestOperators, DivideANumberByZeroWillThrow)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  EXPECT_THROW(any / 0, std::overflow_error);
}

TEST_MEM(AnyTestOperators, DivideANumberByZeroAnyWillThrow)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto zeroany = myodd::dynamic::Any(0);
  EXPECT_THROW(any / zeroany, std::overflow_error);
}

TEST_MEM(AnyTestOperators, DivideANumberByZeroAnyDoubleWillThrow)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto nullany = myodd::dynamic::Any( (double)0.0 );
  EXPECT_THROW(any / nullany, std::overflow_error);
}

TEST_MEM(AnyTestOperators, DivideTwoNullAnyWillThrow)
{
  auto nullany1 = myodd::dynamic::Any(nullptr);
  auto nullany2 = myodd::dynamic::Any(nullptr);
  EXPECT_THROW(nullany1 / nullany2, std::overflow_error);
}

TEST_MEM(AnyTestOperators, ZeroDividedByAnythingIsZero )
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto x = 0 / any;
  ASSERT_EQ(0, x);
}

TEST_MEM(AnyTestOperators, AnyZeroDividedByAnythingIsZero)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto zero = myodd::dynamic::Any(0);
  auto x = zero / any;
  ASSERT_EQ(0, x);
  ASSERT_EQ(zero, x);
}

TEST_MEM(AnyTestOperators, NonNumberStringDividedByAnythingIsZero)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto something = myodd::dynamic::Any("Hello");
  auto x = something / any;
  ASSERT_EQ(0, x);
}

TEST_MEM(AnyTestOperators, NonNumberWideStringDividedByAnythingIsZero)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto something = myodd::dynamic::Any(L"Hello");
  auto x = something / any;
  ASSERT_EQ(0, x);
}

TEST_MEM(AnyTestOperators, NonNumberCharDividedByAnythingIsZero)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto something = myodd::dynamic::Any('&');
  auto x = something / any;
  ASSERT_EQ(0, x);
}

TEST_MEM(AnyTestOperators, NonNumberWideCharDividedByAnythingIsZero)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto something = myodd::dynamic::Any(L'&');
  auto x = something / any;
  ASSERT_EQ(0, x);
}

TEST_MEM(AnyTestOperators, DivideTwoIntegers)
{
  for (;;)
  {
    // get two numbers.
    auto l1 = IntRandomNumber<long>(false);
    auto l2 = IntRandomNumber<long>(false);

    // 
    double d = (double)l1 / (double)l2;

    // the number has to have double with a fraction.
    long double intpart;
    if ( 0 == modf(d, &intpart) )
    {
      continue;
    }

    auto any1 = myodd::dynamic::Any(l1);
    auto any2 = myodd::dynamic::Any(l2);

    auto anyResult = any1 / any2;
    ASSERT_DOUBLE_EQ( d, anyResult);

    // done
    break;
  }
}

TEST_MEM(AnyTestOperators, DivideTwoBoolean)
{
  auto any1 = myodd::dynamic::Any(true);
  auto any2 = myodd::dynamic::Any(true);

  auto anyResult = any1 / any2;
  ASSERT_EQ(1, anyResult);
}

TEST_MEM(AnyTestOperators, DivideCharInteger)
{
  auto any1 = myodd::dynamic::Any('4');
  auto any2 = myodd::dynamic::Any('2');

  auto anyResult = any1 / any2;
  ASSERT_EQ(2, anyResult);

  ASSERT_EQ('4', any1);
  ASSERT_EQ('2', any2);
}

TEST_MEM(AnyTestOperators, DivideWideCharInteger)
{
  auto any1 = myodd::dynamic::Any(L'4');
  auto any2 = myodd::dynamic::Any(L'2');

  auto anyResult = any1 / any2;
  ASSERT_EQ(2, anyResult);

  ASSERT_EQ(L'4', any1);
  ASSERT_EQ(L'2', any2);
}

TEST_MEM(AnyTestOperators, DivideCharDouble)
{
  auto any1 = myodd::dynamic::Any('5');
  auto any2 = myodd::dynamic::Any('2');

  auto anyResult = any1 / any2;
  ASSERT_DOUBLE_EQ(2.5, anyResult);

  ASSERT_EQ('5', any1);
  ASSERT_EQ('2', any2);
}

TEST_MEM(AnyTestOperators, DivideWideCharDouble)
{
  auto any1 = myodd::dynamic::Any(L'5');
  auto any2 = myodd::dynamic::Any(L'2');

  auto anyResult = any1 / any2;
  ASSERT_DOUBLE_EQ(2.5, anyResult);

  ASSERT_EQ(L'5', any1);
  ASSERT_EQ(L'2', any2);
}

TEST_MEM(AnyTestOperators, DivideWideCharAndCharInteger)
{
  auto any1 = myodd::dynamic::Any(L'4');
  auto any2 = myodd::dynamic::Any('2');

  auto anyResult = any1 / any2;
  ASSERT_EQ(2, anyResult);

  ASSERT_EQ(L'4', any1);
  ASSERT_EQ('2', any2);
}

TEST_MEM(AnyTestOperators, DivideWideCharAndCharDouble)
{
  auto any1 = myodd::dynamic::Any(L'5');
  auto any2 = myodd::dynamic::Any('2');

  auto anyResult = any1 / any2;
  ASSERT_DOUBLE_EQ(2.5, anyResult);
  ASSERT_EQ(L'5', any1);
  ASSERT_EQ('2', any2);
}

TEST_MEM(AnyTestOperators, ShortcutDivide)
{
  auto any1 = myodd::dynamic::Any(5);
  auto any2 = myodd::dynamic::Any(2);

  any1 /= any2;
  ASSERT_DOUBLE_EQ(2.5, any1);
  ASSERT_EQ(2, any2);
}

TEST_MEM(AnyTestOperators, DivideStringDouble)
{
  auto any1 = myodd::dynamic::Any("11.9");
  auto any2 = myodd::dynamic::Any("7");

  auto anyResult = any1 / any2;
  ASSERT_DOUBLE_EQ(1.7, anyResult);
}

TEST_MEM(AnyTestOperators, DivideWideStringDouble)
{
  auto any1 = myodd::dynamic::Any(L"11.9");
  auto any2 = myodd::dynamic::Any(L"7");

  auto anyResult = any1 / any2;
  ASSERT_DOUBLE_EQ(1.7, anyResult);
}

TEST_MEM(AnyTestOperators, DivideWideStringAndLongDouble)
{
  auto any1 = myodd::dynamic::Any(L"694.2");
  auto any2 = myodd::dynamic::Any( (long)52 );

  auto anyResult = any1 / any2;
  ASSERT_DOUBLE_EQ(13.35, anyResult);
}

TEST_MEM(AnyTestOperators, DivideTwoStringsWillThrowAnError)
{
  auto any1 = myodd::dynamic::Any("Hello");
  auto any2 = myodd::dynamic::Any("World");
  EXPECT_THROW(any1 / any2, std::overflow_error);
}

TEST_MEM(AnyTestOperators, DivideTwoWideStringsWillThrowAnError)
{
  auto any1 = myodd::dynamic::Any(L"Hello");
  auto any2 = myodd::dynamic::Any(L"World");
  EXPECT_THROW(any1 / any2, std::overflow_error);
}

TEST_MEM(AnyTestOperators, QuickAddPrefix)
{
  auto number = IntRandomNumber<short>(false);
  auto any = number;
  ASSERT_EQ( any, number);

  ASSERT_EQ(++any, (int)number+1);
  ASSERT_EQ( any, (int)number+1);
}

TEST_MEM(AnyTestOperators, QuickSubPrefix)
{
  auto number = IntRandomNumber<short>(false);
  auto any = myodd::dynamic::Any(number);
  ASSERT_EQ(any, number);

  ASSERT_EQ(--any, (number-1));
  ASSERT_EQ(any, (number-1));
}

TEST_MEM(AnyTestOperators, QuickAddPostFix)
{
  auto number = IntRandomNumber<short>(false);
  auto any = myodd::dynamic::Any(number);
  ASSERT_EQ(any, number);

  ASSERT_EQ(any++, number);
  ASSERT_EQ(any, (number + 1));
}

TEST_MEM(AnyTestOperators, QuickSubPostFix)
{
  auto number = IntRandomNumber<short>(false);
  auto any = myodd::dynamic::Any(number);
  ASSERT_EQ(any, number);

  ASSERT_EQ(any--, number);
  ASSERT_EQ(any, (number - 1));
}

///////////////////////////////////////////////////////////////

TEST_MEM(AnyTestOperators, AddShortIntToAny)
{
  auto start = IntRandomNumber<short int>(false);
  auto number = IntRandomNumber<short int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any += number;
  ASSERT_EQ(any, (start+number));

  any = myodd::dynamic::Any(start);
  any = any+number;
  ASSERT_EQ(any, (start + number));
}

TEST_MEM(AnyTestOperators, AddUnsignedShortIntToAny)
{
  auto start = IntRandomNumber<unsigned short int>(false);
  auto number = IntRandomNumber<unsigned short int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any += number;
  ASSERT_EQ(any, (start + number));

  any = myodd::dynamic::Any(start);
  any = any + number;
  ASSERT_EQ(any, (start + number));
}

TEST_MEM(AnyTestOperators, AddIntToAny)
{
  auto start = IntRandomNumber<int>(false);
  auto number = IntRandomNumber<int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any += number;
  ASSERT_EQ(any, (start + number));

  any = myodd::dynamic::Any(start);
  any = any + number;
  ASSERT_EQ(any, (start + number));
}

TEST_MEM(AnyTestOperators, AddUnsignedIntToAny)
{
  auto start = IntRandomNumber<unsigned int>(false);
  auto number = IntRandomNumber<unsigned int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any += number;
  ASSERT_EQ(any, (start + number));

  any = myodd::dynamic::Any(start);
  any = any + number;
  ASSERT_EQ(any, (start + number));
}

TEST_MEM(AnyTestOperators, AddLongIntToAny)
{
  auto start = IntRandomNumber <long int > (false);
  auto number = IntRandomNumber<long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any += number;
  ASSERT_EQ(any, (start + number));

  any = myodd::dynamic::Any(start);
  any = any + number;
  ASSERT_EQ(any, (start + number));
}

TEST_MEM(AnyTestOperators, AddUnsignedLongIntToAny)
{
  auto start = IntRandomNumber <unsigned long int >(false);
  auto number = IntRandomNumber<unsigned long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any += number;
  ASSERT_EQ(any, (start + number));

  any = myodd::dynamic::Any(start);
  any = any + number;
  ASSERT_EQ(any, (start + number));
}

TEST_MEM(AnyTestOperators, AddLongLongIntToAny)
{
  auto start = IntRandomNumber <long long int >(false);
  auto number = IntRandomNumber<long long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any += number;
  ASSERT_EQ(any, (start + number));

  any = myodd::dynamic::Any(start);
  any = any + number;
  ASSERT_EQ(any, (start + number));
}

TEST_MEM(AnyTestOperators, AddUnsignedLongLongIntToAny)
{
  auto start = IntRandomNumber <unsigned long long int >(false);
  auto number = IntRandomNumber<unsigned long long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any += number;
  ASSERT_EQ(any, (start + number));

  any = myodd::dynamic::Any(start);
  any = any + number;
  ASSERT_EQ(any, (start + number));
}

TEST_MEM(AnyTestOperators, AddFloatToAny)
{
  auto start = RealRandomNumber <float>(false);
  auto number = RealRandomNumber<float>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any += number;
  ASSERT_EQ(any, (start + number));

  any = myodd::dynamic::Any(start);
  any = any + number;
  ASSERT_EQ(any, (start + number));
}

TEST_MEM(AnyTestOperators, AddDoubleToAny)
{
  auto start = RealRandomNumber <double>(false);
  auto number = RealRandomNumber<double>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any += number;
  ASSERT_EQ(any, (start + number));

  any = myodd::dynamic::Any(start);
  any = any + number;
  ASSERT_EQ(any, (start + number));
}

TEST_MEM(AnyTestOperators, AddLongDoubleToAny)
{
  auto start = RealRandomNumber <long  double>(false);
  auto number = RealRandomNumber<long double>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any += number;
  ASSERT_EQ(any, (start + number));

  any = myodd::dynamic::Any(start);
  any = any + number;
  ASSERT_EQ(any, (start + number));
}

///////////////////////////////////////////////////////////////

TEST_MEM(AnyTestOperators, SubstractShortIntToAny)
{
  auto start = IntRandomNumber<short int>(false);
  auto number = IntRandomNumber<short int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any -= number;
  ASSERT_EQ(any, (start - number));

  any = myodd::dynamic::Any(start);
  any = any - number;
  ASSERT_EQ(any, (start - number));
}

TEST_MEM(AnyTestOperators, SubstractUnsignedShortIntToAny)
{
  auto start = IntRandomNumber<unsigned short int>(false);
  auto number = IntRandomNumber<unsigned short int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any -= number;
  ASSERT_EQ(any, (start - number));

  any = myodd::dynamic::Any(start);
  any = any - number;
  ASSERT_EQ(any, (start - number));
}

TEST_MEM(AnyTestOperators, SubstractIntToAny)
{
  auto start = IntRandomNumber<int>(false);
  auto number = IntRandomNumber<int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any -= number;
  ASSERT_EQ(any, (start - number));

  any = myodd::dynamic::Any(start);
  any = any - number;
  ASSERT_EQ(any, (start - number));
}

TEST_MEM(AnyTestOperators, SubstractUnsignedIntToAny)
{
  auto start = IntRandomNumber<unsigned int>(false);
  auto number = IntRandomNumber<unsigned int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any -= number;
  ASSERT_EQ(any, (start - number));

  any = myodd::dynamic::Any(start);
  any = any - number;
  ASSERT_EQ(any, (start - number));
}

TEST_MEM(AnyTestOperators, SubstractLongIntToAny)
{
  auto start = IntRandomNumber <long int >(false);
  auto number = IntRandomNumber<long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any -= number;
  ASSERT_EQ(any, (start - number));

  any = myodd::dynamic::Any(start);
  any = any - number;
  ASSERT_EQ(any, (start - number));
}

TEST_MEM(AnyTestOperators, SubstractUnsignedLongIntToAny)
{
  auto start = IntRandomNumber <unsigned long int >(false);
  auto number = IntRandomNumber<unsigned long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any -= number;
  ASSERT_EQ(any, (start - number));

  any = myodd::dynamic::Any(start);
  any = any - number;
  ASSERT_EQ(any, (start - number));
}

TEST_MEM(AnyTestOperators, SubstractLongLongIntToAny)
{
  auto start = IntRandomNumber <long long int >(false);
  auto number = IntRandomNumber<long long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any -= number;
  ASSERT_EQ(any, (start - number));

  any = myodd::dynamic::Any(start);
  any = any - number;
  ASSERT_EQ(any, (start - number));
}

TEST_MEM(AnyTestOperators, SubstractUnsignedLongLongIntToAny)
{
  auto start = IntRandomNumber <unsigned long long int >(false);
  auto number = IntRandomNumber<unsigned long long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any -= number;
  ASSERT_EQ(any, (start - number));

  any = myodd::dynamic::Any(start);
  any = any - number;
  ASSERT_EQ(any, (start - number));
}

///////////////////////////////////////////////////////////////

TEST_MEM(AnyTestOperators, MultiplyShortIntToAny)
{
  auto start = IntRandomNumber<short int>(false);
  auto number = IntRandomNumber<short int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any *= number;
  ASSERT_EQ(any, (start * number));

  any = myodd::dynamic::Any(start);
  any = any * number;
  ASSERT_EQ(any, (start * number));
}

TEST_MEM(AnyTestOperators, MultiplyUnsignedShortIntToAny)
{
  auto start = IntRandomNumber<unsigned short int>(false);
  auto number = IntRandomNumber<unsigned short int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any *= number;
  ASSERT_EQ(any, (start * number));

  any = myodd::dynamic::Any(start);
  any = any * number;
  ASSERT_EQ(any, (start * number));
}

TEST_MEM(AnyTestOperators, MultiplyIntToAny)
{
  auto start = IntRandomNumber<int>(false);
  auto number = IntRandomNumber<int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any *= number;
  ASSERT_EQ(any, (start * number));

  any = myodd::dynamic::Any(start);
  any = any * number;
  ASSERT_EQ(any, (start * number));
}

TEST_MEM(AnyTestOperators, MultiplyUnsignedIntToAny)
{
  auto start = IntRandomNumber<unsigned int>(false);
  auto number = IntRandomNumber<unsigned int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any *= number;
  ASSERT_EQ(any, (start * number));

  any = myodd::dynamic::Any(start);
  any = any * number;
  ASSERT_EQ(any, (start * number));
}

TEST_MEM(AnyTestOperators, MultiplyLongIntToAny)
{
  auto start = IntRandomNumber <long int >(false);
  auto number = IntRandomNumber<long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any *= number;
  ASSERT_EQ(any, (start * number));

  any = myodd::dynamic::Any(start);
  any = any * number;
  ASSERT_EQ(any, (start * number));
}

TEST_MEM(AnyTestOperators, MultiplyUnsignedLongIntToAny)
{
  auto start = IntRandomNumber <unsigned long int >(false);
  auto number = IntRandomNumber<unsigned long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any *= number;
  ASSERT_EQ(any, (start * number));

  any = myodd::dynamic::Any(start);
  any = any * number;
  ASSERT_EQ(any, (start * number));
}

TEST_MEM(AnyTestOperators, MultiplyLongLongIntToAny)
{
  auto start = IntRandomNumber <long long int >(false);
  auto number = IntRandomNumber<long long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any *= number;
  ASSERT_EQ(any, (start * number));

  any = myodd::dynamic::Any(start);
  any = any * number;
  ASSERT_EQ(any, (start * number));
}

TEST_MEM(AnyTestOperators, MultiplyUnsignedLongLongIntToAny)
{
  auto start = IntRandomNumber <unsigned long long int >(false);
  auto number = IntRandomNumber<unsigned long long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any *= number;
  ASSERT_EQ(any, (start * number));

  any = myodd::dynamic::Any(start);
  any = any * number;
  ASSERT_EQ(any, (start * number));
}

///////////////////////////////////////////////////////////////

TEST_MEM(AnyTestOperators, DivideShortIntToAny)
{
  auto start = IntRandomNumber<short int>(false);
  auto number = IntRandomNumber<short int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any /= number;
  ASSERT_EQ(any, (start / number));

  any = myodd::dynamic::Any(start);
  any = any / number;
  ASSERT_EQ(any, (start / number));
}

TEST_MEM(AnyTestOperators, DivideUnsignedShortIntToAny)
{
  auto start = IntRandomNumber<unsigned short int>(false);
  auto number = IntRandomNumber<unsigned short int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any /= number;
  ASSERT_EQ(any, (start / number));

  any = myodd::dynamic::Any(start);
  any = any / number;
  ASSERT_EQ(any, (start / number));
}

TEST_MEM(AnyTestOperators, DivideIntToAny)
{
  auto start = IntRandomNumber<int>(false);
  auto number = IntRandomNumber<int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any /= number;
  ASSERT_EQ(any, (start / number));

  any = myodd::dynamic::Any(start);
  any = any / number;
  ASSERT_EQ(any, (start / number));
}

TEST_MEM(AnyTestOperators, DivideUnsignedIntToAny)
{
  auto start = IntRandomNumber<unsigned int>(false);
  auto number = IntRandomNumber<unsigned int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any /= number;
  ASSERT_EQ(any, (start / number));

  any = myodd::dynamic::Any(start);
  any = any / number;
  ASSERT_EQ(any, (start / number));
}

TEST_MEM(AnyTestOperators, DivideLongIntToAny)
{
  auto start = IntRandomNumber <long int >(false);
  auto number = IntRandomNumber<long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any /= number;
  ASSERT_EQ(any, (start / number));

  any = myodd::dynamic::Any(start);
  any = any / number;
  ASSERT_EQ(any, (start / number));
}

TEST_MEM(AnyTestOperators, DivideUnsignedLongIntToAny)
{
  auto start = IntRandomNumber <unsigned long int >(false);
  auto number = IntRandomNumber<unsigned long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any /= number;
  ASSERT_EQ(any, (start / number));

  any = myodd::dynamic::Any(start);
  any = any / number;
  ASSERT_EQ(any, (start / number));
}

TEST_MEM(AnyTestOperators, DivideLongLongIntToAny)
{
  auto start = IntRandomNumber <long long int >(false);
  auto number = IntRandomNumber<long long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any /= number;
  ASSERT_EQ(any, (start / number));

  any = myodd::dynamic::Any(start);
  any = any / number;
  ASSERT_EQ(any, (start / number));
}

TEST_MEM(AnyTestOperators, DivideUnsignedLongLongIntToAny)
{
  auto start = IntRandomNumber <unsigned long long int >(false);
  auto number = IntRandomNumber<unsigned long long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any /= number;
  ASSERT_EQ(any, ((long double)start / (long double)number));

  any = myodd::dynamic::Any(start);
  any = any / number;
  ASSERT_EQ(any, ((long double)start / (long double)number));
}