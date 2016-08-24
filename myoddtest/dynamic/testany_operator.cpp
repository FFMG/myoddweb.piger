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
  ASSERT_TRUE(x == y);
}

TEST(AnyTestOperators, TwoNullValuesAreNotMoreThanEachOthers)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(nullptr);
  ASSERT_FALSE(x > y);
  ASSERT_TRUE(x == y);
}

TEST(AnyTestOperators, TwoNullValuesCanBeLessOrEqual)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(nullptr);
  ASSERT_TRUE(x <= y);
  ASSERT_TRUE(x == y);
}

TEST(AnyTestOperators, TwoNullValuesCanBeMoreOrEqual)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(nullptr);
  ASSERT_TRUE(x >= y);
  ASSERT_TRUE(x == y);
}

TEST(AnyTestOperators, NullLhsIsLessThanBigNumber)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any( IntRandomNumber<unsigned int>( false ) );
  ASSERT_TRUE(x < y);
  ASSERT_FALSE(x > y);
  ASSERT_FALSE(x >= y);
}

TEST(AnyTestOperators, NullRhsIsLessThanSmallNegNumber)
{
  //  -v1 number ...
  auto i = IntRandomNumber<int>(false);
  auto x = myodd::dynamic::Any( i > 0 ? -1*i : i );
  auto y = myodd::dynamic::Any(nullptr);
  ASSERT_TRUE(x < y);
  ASSERT_FALSE(x > y);
  ASSERT_FALSE(x >= y);
}

TEST(AnyTestOperators, NullEqualsAnyNul )
{
  auto x = myodd::dynamic::Any(nullptr);
  ASSERT_TRUE( x == nullptr );
  ASSERT_TRUE(nullptr ==  x);
}

TEST(AnyTestOperators, NullPlusNullEqualZero)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(nullptr);
  auto z = x + y;
  ASSERT_EQ( 0, z);
  ASSERT_EQ( myodd::dynamic::Integer_int, z.Type() );
  ASSERT_EQ(nullptr, x);
  ASSERT_EQ(myodd::dynamic::type_null, x.Type());
  ASSERT_EQ(nullptr, y);
  ASSERT_EQ(myodd::dynamic::type_null, y.Type());
}

TEST(AnyTestOperators, NullMinusNullEqualZero)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(nullptr);
  auto z = x - y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(myodd::dynamic::Integer_int, z.Type());
  ASSERT_EQ(nullptr, x);
  ASSERT_EQ(myodd::dynamic::type_null, x.Type());
  ASSERT_EQ(nullptr, y);
  ASSERT_EQ(myodd::dynamic::type_null, y.Type());
}

TEST(AnyTestOperators, NullPlusNullEqualZeroShortcut)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(nullptr);
  x += y;

  // x is updated
  ASSERT_EQ(0, x);
  ASSERT_EQ(myodd::dynamic::Integer_int, x.Type());

  // y has not changed
  ASSERT_EQ(nullptr, y);
  ASSERT_EQ(myodd::dynamic::type_null, y.Type());
}

TEST(AnyTestOperators, NullMinusNullEqualZeroShortcut)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(nullptr);
  x -= y;

  // x is updated
  ASSERT_EQ(0, x);
  ASSERT_EQ(myodd::dynamic::Integer_int, x.Type());

  // y has not changed
  ASSERT_EQ(nullptr, y);
  ASSERT_EQ(myodd::dynamic::type_null, y.Type());
}

TEST(AnyTestOperators, AddNumberToNull)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(10);
  x += y;
  ASSERT_EQ(10, x);
}

TEST(AnyTestOperators, SubtractNumberFromNull)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(10);
  x -= y;
  ASSERT_EQ(-10, x);
}

TEST(AnyTestOperators, MultiplyNumberAndNull)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(IntRandomNumber<int>(false));
  x *= y;
  ASSERT_EQ(0, x);
}

TEST(AnyTestOperators, AddNullToNumber)
{
  auto x = myodd::dynamic::Any(10);
  auto y = myodd::dynamic::Any(nullptr);
  x += y;
  ASSERT_EQ(10, x);
}

TEST(AnyTestOperators, SubstractNullFromNumber)
{
  auto x = myodd::dynamic::Any(10);
  auto y = myodd::dynamic::Any(nullptr);
  x -= y;
  ASSERT_EQ(10, x);
}

TEST(AnyTestOperators, MultiplyNullFromNumber)
{
  auto x = myodd::dynamic::Any(IntRandomNumber<int>(false));
  auto y = myodd::dynamic::Any(nullptr);
  x *= y;
  ASSERT_EQ(0, x);
}

TEST(AnyTestOperators, AddTwoAny)
{
  auto x = myodd::dynamic::Any(10);
  auto y = myodd::dynamic::Any(20);
  x += y;
  ASSERT_EQ(30, x);
}

TEST(AnyTestOperators, SubstractTwoAny)
{
  auto x = myodd::dynamic::Any(40);
  auto y = myodd::dynamic::Any(10);
  x -= y;
  ASSERT_EQ(30, x);
}

TEST(AnyTestOperators, MultiplyTwoAny)
{
  auto x = myodd::dynamic::Any(40);
  auto y = myodd::dynamic::Any(10);
  x *= y;
  ASSERT_EQ(400, x);
}

TEST(AnyTestOperators, AddANumberToAny)
{
  auto x = myodd::dynamic::Any(10);
  x += 20;
  ASSERT_EQ(30, x);
}

TEST(AnyTestOperators, MultiplyANumberToAny)
{
  auto x = myodd::dynamic::Any(40);
  x -= 10;
  ASSERT_EQ(30, x);
}

TEST(AnyTestOperators, SubstractANumberToAny)
{
  auto x = myodd::dynamic::Any(40);
  x *= 10;
  ASSERT_EQ(400, x);
}

TEST(AnyTestOperators, AddAnyToANumber)
{
  auto x = myodd::dynamic::Any(10);
  x = 20 + x;
  ASSERT_EQ(30, x);
}

TEST(AnyTestOperators, SubstractAnyToANumber)
{
  auto x = myodd::dynamic::Any(10);
  x = 40 - x;
  ASSERT_EQ(30, x);
}

TEST(AnyTestOperators, MultiplyAnyToANumber)
{
  auto x = myodd::dynamic::Any(10);
  x = 40 * x;
  ASSERT_EQ(400, x);
}

TEST(AnyTestOperators, AddAnyToANumberAndCreate)
{
  auto x = myodd::dynamic::Any(10);
  auto y = 20 + x;
  ASSERT_EQ(30, y);
}

TEST(AnyTestOperators, SubstractAnyToANumberAndCreate)
{
  auto x = myodd::dynamic::Any(10);
  auto y = 40 - x;
  ASSERT_EQ(30, y);
}

TEST(AnyTestOperators, MultiplyAnyToANumberAndCreate)
{
  auto x = myodd::dynamic::Any(10);
  auto y = 40 * x;
  ASSERT_EQ(400, y);
}

TEST(AnyTestOperators, AddTwoDifferentTypes )
{
  auto x = myodd::dynamic::Any( (int)10 );
  auto y = myodd::dynamic::Any( (double)20.5 );
  auto z = x + y;
  ASSERT_EQ(30.5, z );
  ASSERT_EQ(10, x);
  ASSERT_EQ(20.5, y);
}

TEST(AnyTestOperators, SubstractwoDifferentTypes)
{
  auto x = myodd::dynamic::Any((int)30);
  auto y = myodd::dynamic::Any((double)10.5);
  auto z = x - y;
  ASSERT_EQ(19.5, z);
  ASSERT_EQ(30, x);
  ASSERT_EQ(10.5, y);
}

TEST(AnyTestOperators, MultiplytwoDifferentTypes)
{
  auto x = myodd::dynamic::Any((int)30);
  auto y = myodd::dynamic::Any((double)10.5);
  auto z = x * y;
  ASSERT_EQ((double)315, z);// we multiplied a double * int so z is a double.
  ASSERT_EQ(30, x);
  ASSERT_EQ(10.5, y);
}

TEST(AnyTestOperators, AddTwoBlooean)
{
  auto x = myodd::dynamic::Any( true );
  auto y = myodd::dynamic::Any( false );
  auto z = x + y;
  ASSERT_EQ(1, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(false, y);
}

TEST(AnyTestOperators, SubstractTwoBlooean)
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

TEST(AnyTestOperators, MultiplyTwoBlooean)
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

TEST(AnyTestOperators, MultiplyTwoTrueBlooean)
{
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(true);
  auto z = x * y;
  ASSERT_EQ(1, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(true, y);
}

TEST(AnyTestOperators, AddANumberToABoolean)
{
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(10);
  auto z = x + y;
  ASSERT_EQ(11, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(10, y);
}

TEST(AnyTestOperators, SubstractANumberFromABoolean)
{
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(10);
  auto z = x - y;
  ASSERT_EQ(-9, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(10, y);
}

TEST(AnyTestOperators, MultiplyANumberAndABoolean)
{
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(10);
  auto z = x * y;
  ASSERT_EQ(10, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(10, y);
}

TEST(AnyTestOperators, AddNullToABooleanBecomesAnInt)
{
  auto x = myodd::dynamic::Any( nullptr );
  auto y = myodd::dynamic::Any( true );
  auto z = x + y;
  ASSERT_EQ(1, z);
  ASSERT_EQ(nullptr, x);
  ASSERT_EQ(true, y);
}

TEST(AnyTestOperators, SubstractNullFromABooleanBecomesAnInt)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(true);
  auto z = x - y;
  ASSERT_EQ(-1, z);
  ASSERT_EQ(nullptr, x);
  ASSERT_EQ(true, y);
}

TEST(AnyTestOperators, MultiplyNullAndABooleanBecomesAnInt)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(true);
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(nullptr, x);
  ASSERT_EQ(true, y);
}

TEST(AnyTestOperators, AddBooleanToNullBecomesAnInt)
{
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(nullptr);
  auto z = x + y;
  ASSERT_EQ(1, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(nullptr, y);
}

TEST(AnyTestOperators, SubstractBooleanFromNullBecomesAnInt)
{
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(nullptr);
  auto z = x - y;
  ASSERT_EQ(1, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(nullptr, y);
}

TEST(AnyTestOperators, MultiplyBooleanAndNullBecomesAnInt)
{
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(nullptr);
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(true, x);
  ASSERT_EQ(nullptr, y);
}

TEST(AnyTestOperators, ChainNumberAddition)
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

TEST(AnyTestOperators, ChainNumberSubstraction)
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

TEST(AnyTestOperators, ChainNumberMultiplication)
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

TEST(AnyTestOperators, AddTwoCharOnlyIfTheCharIsANumber)
{
  auto x = myodd::dynamic::Any('1');
  auto y = myodd::dynamic::Any('9' );
  auto z = x + y;
  ASSERT_EQ(10, z);
  ASSERT_EQ('1', x);
  ASSERT_EQ('9', y);
}

TEST(AnyTestOperators, SubstractTwoCharOnlyIfTheCharIsANumber)
{
  auto x = myodd::dynamic::Any('9');
  auto y = myodd::dynamic::Any('1');
  auto z = x - y;
  ASSERT_EQ( 8, z);
  ASSERT_EQ('9', x);
  ASSERT_EQ('1', y);
}

TEST(AnyTestOperators, MultiplyTwoCharOnlyIfTheCharIsANumber)
{
  auto x = myodd::dynamic::Any('9');
  auto y = myodd::dynamic::Any('2');
  auto z = x * y;
  ASSERT_EQ(18, z);
  ASSERT_EQ('9', x);
  ASSERT_EQ('2', y);
}

TEST(AnyTestOperators, AddTwoWideCharOnlyIfTheCharIsANumber)
{
  auto x = myodd::dynamic::Any(L'1');
  auto y = myodd::dynamic::Any(L'9');
  auto z = x + y;
  ASSERT_EQ(10, z);
  ASSERT_EQ(L'1', x);
  ASSERT_EQ(L'9', y);
}

TEST(AnyTestOperators, SubstractTwoWideCharOnlyIfTheCharIsANumber)
{
  auto x = myodd::dynamic::Any(L'9');
  auto y = myodd::dynamic::Any(L'1');
  auto z = x - y;
  ASSERT_EQ( 8, z);
  ASSERT_EQ(L'9', x);
  ASSERT_EQ(L'1', y);
}

TEST(AnyTestOperators, MultiplyTwoWideCharOnlyIfTheCharIsANumber)
{
  auto x = myodd::dynamic::Any(L'9');
  auto y = myodd::dynamic::Any(L'2');
  auto z = x * y;
  ASSERT_EQ(18, z);
  ASSERT_EQ(L'9', x);
  ASSERT_EQ(L'2', y);
}

TEST(AnyTestOperators, AddTwoCharZeroIfNoneAreNumbers)
{
  auto x = myodd::dynamic::Any('a');
  auto y = myodd::dynamic::Any('&');
  auto z = x + y;
  ASSERT_EQ(0, z);
  ASSERT_EQ('a', x);
  ASSERT_EQ('&', y);
}

TEST(AnyTestOperators, SubstractTwoCharZeroIfNoneAreNumbers)
{
  auto x = myodd::dynamic::Any('a');
  auto y = myodd::dynamic::Any('&');
  auto z = x - y;
  ASSERT_EQ(0, z);
  ASSERT_EQ('a', x);
  ASSERT_EQ('&', y);
}

TEST(AnyTestOperators, MultiplyTwoCharZeroIfNoneAreNumbers)
{
  auto x = myodd::dynamic::Any('a');
  auto y = myodd::dynamic::Any('&');
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ('a', x);
  ASSERT_EQ('&', y);
}

TEST(AnyTestOperators, AddTwoWideCharZeroIfNoneAreNumbers)
{
  auto x = myodd::dynamic::Any(L'a');
  auto y = myodd::dynamic::Any(L'&');
  auto z = x + y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(L'a', x);
  ASSERT_EQ(L'&', y);
}

TEST(AnyTestOperators, SubstractTwoWideCharZeroIfNoneAreNumbers)
{
  auto x = myodd::dynamic::Any(L'a');
  auto y = myodd::dynamic::Any(L'&');
  auto z = x - y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(L'a', x);
  ASSERT_EQ(L'&', y);
}

TEST(AnyTestOperators, MultiplyTwoWideCharZeroIfNoneAreNumbers)
{
  auto x = myodd::dynamic::Any(L'a');
  auto y = myodd::dynamic::Any(L'&');
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(L'a', x);
  ASSERT_EQ(L'&', y);
}

TEST(AnyTestOperators, AddTwoCharIfOneIsANumber)
{
  auto x = myodd::dynamic::Any('1');
  auto y = myodd::dynamic::Any('&');
  auto z = x + y;
  ASSERT_EQ(1, z);
  ASSERT_EQ('1', x);
  ASSERT_EQ('&', y);
}

TEST(AnyTestOperators, AddTwoStringIfOneIsANumber)
{
  auto x = myodd::dynamic::Any("1234");
  auto y = myodd::dynamic::Any("Hello");
  auto z = x + y;
  ASSERT_EQ(1234, z);
  ASSERT_EQ("1234", x);
  ASSERT_EQ("Hello", y);
}

TEST(AnyTestOperators, SubtractTwoCharIfOneIsANumber)
{
  auto x = myodd::dynamic::Any('1');
  auto y = myodd::dynamic::Any('&');
  auto z = x - y;
  ASSERT_EQ(1, z);
  ASSERT_EQ('1', x);
  ASSERT_EQ('&', y);
}

TEST(AnyTestOperators, SubstractTwoStringIfOneIsANumber)
{
  auto x = myodd::dynamic::Any("1234");
  auto y = myodd::dynamic::Any("Hello");
  auto z = x - y;
  ASSERT_EQ(1234, z);
  ASSERT_EQ("1234", x);
  ASSERT_EQ("Hello", y);
}

TEST(AnyTestOperators, MultiplyTwoCharIfOneIsANumber)
{
  auto x = myodd::dynamic::Any('1');
  auto y = myodd::dynamic::Any('&');
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ('1', x);
  ASSERT_EQ('&', y);
}

TEST(AnyTestOperators, MultiplyTwoStringIfOneIsANumber)
{
  auto x = myodd::dynamic::Any("1234");
  auto y = myodd::dynamic::Any("Hello");
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ("1234", x);
  ASSERT_EQ("Hello", y);
}

TEST(AnyTestOperators, AddTwoWideCharIfOneIsANumber)
{
  auto x = myodd::dynamic::Any(L'1');
  auto y = myodd::dynamic::Any(L'&');
  auto z = x + y;
  ASSERT_EQ(1, z);
  ASSERT_EQ(L'1', x);
  ASSERT_EQ(L'&', y);
}

TEST(AnyTestOperators, AddTwoWideStringIfOneIsANumber)
{
  auto x = myodd::dynamic::Any(L"1234");
  auto y = myodd::dynamic::Any(L"Hello");
  auto z = x + y;
  ASSERT_EQ(1234, z);
  ASSERT_EQ(L"1234", x);
  ASSERT_EQ(L"Hello", y);
}

TEST(AnyTestOperators, SubstractTwoWideStringIfOneIsANumber)
{
  auto x = myodd::dynamic::Any(L"1234");
  auto y = myodd::dynamic::Any(L"Hello");
  auto z = x - y;
  ASSERT_EQ(1234, z);
  ASSERT_EQ(L"1234", x);
  ASSERT_EQ(L"Hello", y);
}

TEST(AnyTestOperators, SubstractTwoWideCharIfOneIsANumber)
{
  auto x = myodd::dynamic::Any(L'1');
  auto y = myodd::dynamic::Any(L'&');
  auto z = x - y;
  ASSERT_EQ(1, z);
  ASSERT_EQ(L'1', x);
  ASSERT_EQ(L'&', y);
}

TEST(AnyTestOperators, MultiplyTwoWideStringIfOneIsANumber)
{
  auto x = myodd::dynamic::Any(L"1234");
  auto y = myodd::dynamic::Any(L"Hello");
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(L"1234", x);
  ASSERT_EQ(L"Hello", y);
}

TEST(AnyTestOperators, MultiplyTwoWideCharIfOneIsANumber)
{
  auto x = myodd::dynamic::Any(L'1');
  auto y = myodd::dynamic::Any(L'&');
  auto z = x * y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(L'1', x);
  ASSERT_EQ(L'&', y);
}

TEST(AnyTestOperators, AddACharAndAWideChar )
{
  auto x = myodd::dynamic::Any(L'1');
  auto y = myodd::dynamic::Any('2');
  auto z = x + y;
  ASSERT_EQ(3, z);
  ASSERT_EQ(L'1', x);
  ASSERT_EQ('2', y);
}

TEST(AnyTestOperators, SubstractACharAndAWideChar)
{
  auto x = myodd::dynamic::Any(L'4');
  auto y = myodd::dynamic::Any('1');
  auto z = x - y;
  ASSERT_EQ(3, z);
  ASSERT_EQ(L'4', x);
  ASSERT_EQ('1', y);
}

TEST(AnyTestOperators, MultiplyACharAndAWideChar)
{
  auto x = myodd::dynamic::Any(L'4');
  auto y = myodd::dynamic::Any('2');
  auto z = x * y;
  ASSERT_EQ(8, z);
  ASSERT_EQ(L'4', x);
  ASSERT_EQ('2', y);
}

TEST(AnyTestOperators, AdditionAndSubstraction)
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

TEST(AnyTestOperators, AdditionAndSubstractionReUseNumbers)
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

TEST(AnyTestOperators, MultiplicationAdditionAndSubstractionReUseNumbers)
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

TEST(AnyTestOperators, MultiplicationAdditionAndSubstractionParentheseReUseNumbers)
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

TEST(AnyTestOperators, DivideANumberByNullWillThrow)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>());
  EXPECT_THROW(any / nullptr, std::overflow_error);
}

TEST(AnyTestOperators, DivideZeroByNullWillThrow)
{
  auto any = myodd::dynamic::Any(0);
  EXPECT_THROW(any / nullptr, std::overflow_error);
}

TEST(AnyTestOperators, DivideANumberByNullAnyWillThrow)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto nullany = myodd::dynamic::Any( nullptr );
  EXPECT_THROW(any / nullany, std::overflow_error);
}

TEST(AnyTestOperators, DivideANumberByZeroWillThrow)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  EXPECT_THROW(any / 0, std::overflow_error);
}

TEST(AnyTestOperators, DivideANumberByZeroAnyWillThrow)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto zeroany = myodd::dynamic::Any(0);
  EXPECT_THROW(any / zeroany, std::overflow_error);
}

TEST(AnyTestOperators, DivideANumberByZeroAnyDoubleWillThrow)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto nullany = myodd::dynamic::Any( (double)0.0 );
  EXPECT_THROW(any / nullany, std::overflow_error);
}

TEST(AnyTestOperators, DivideTwoNullAnyWillThrow)
{
  auto nullany1 = myodd::dynamic::Any(nullptr);
  auto nullany2 = myodd::dynamic::Any(nullptr);
  EXPECT_THROW(nullany1 / nullany2, std::overflow_error);
}

TEST(AnyTestOperators, ZeroDividedByAnythingIsZero )
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto x = 0 / any;
  ASSERT_EQ(0, x);
}

TEST(AnyTestOperators, AnyZeroDividedByAnythingIsZero)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto zero = myodd::dynamic::Any(0);
  auto x = zero / any;
  ASSERT_EQ(0, x);
  ASSERT_EQ(zero, x);
}

TEST(AnyTestOperators, NonNumberStringDividedByAnythingIsZero)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto something = myodd::dynamic::Any("Hello");
  auto x = something / any;
  ASSERT_EQ(0, x);
}

TEST(AnyTestOperators, NonNumberWideStringDividedByAnythingIsZero)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto something = myodd::dynamic::Any(L"Hello");
  auto x = something / any;
  ASSERT_EQ(0, x);
}

TEST(AnyTestOperators, NonNumberCharDividedByAnythingIsZero)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto something = myodd::dynamic::Any('&');
  auto x = something / any;
  ASSERT_EQ(0, x);
}

TEST(AnyTestOperators, NonNumberWideCharDividedByAnythingIsZero)
{
  auto any = myodd::dynamic::Any(IntRandomNumber<long>(false));
  auto something = myodd::dynamic::Any(L'&');
  auto x = something / any;
  ASSERT_EQ(0, x);
}
