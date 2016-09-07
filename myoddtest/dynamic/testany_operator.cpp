#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestOperators* 
TEST_MEM(AnyTestOperators, LessThan2xAny)
{
  auto x = myodd::dynamic::Any(10);
  auto y = myodd::dynamic::Any(20);
  ASSERT_TRUE(x < y);
}

TEST_MEM(AnyTestOperators, LessThanAnyLessOther)
{
  auto x = myodd::dynamic::Any(10);
  ASSERT_TRUE(x < 20);
}

TEST_MEM(AnyTestOperators, LessThanOtherLessThanAny)
{
  auto x = myodd::dynamic::Any(10);
  ASSERT_TRUE(5 < x);
}

TEST_MEM(AnyTestOperators, LessThanAnyMoreThanOther)
{
  auto x = myodd::dynamic::Any(50);
  ASSERT_TRUE(x > 20);
}

TEST_MEM(AnyTestOperators, OtherMoreThanAny)
{
  auto x = myodd::dynamic::Any(10);
  ASSERT_TRUE(20 > x);
}

TEST_MEM(AnyTestOperators, OtherNotMoreThanAnyWhenAnyEqualsOther)
{
  auto x = myodd::dynamic::Any(20);
  ASSERT_FALSE(20 > x); // 20 not > than 20
}

TEST_MEM(AnyTestOperators, MoreThan2xAny)
{
  auto x = myodd::dynamic::Any(50);
  auto y = myodd::dynamic::Any(20);
  ASSERT_TRUE(x > y);
}

TEST_MEM(AnyTestOperators, MoreThanOrEqual2xAnyWhenBothAreEqual)
{
  auto x = myodd::dynamic::Any(20);
  auto y = myodd::dynamic::Any(20);
  ASSERT_TRUE(x >= y);
}

TEST_MEM(AnyTestOperators, TwoNullValuesAreNotLessThanEachOthers)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(nullptr);
  ASSERT_FALSE(x < y);
  ASSERT_FALSE(x > y);
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
  auto y = myodd::dynamic::Any(IntRandomNumber<unsigned int>(false));
  ASSERT_TRUE(x < y);
  ASSERT_FALSE(x > y);
  ASSERT_FALSE(x >= y);
}

TEST_MEM(AnyTestOperators, NullRhsIsLessThanSmallNegNumber)
{
  //  -v1 number ...
  auto i = IntRandomNumber<int>(false);
  i = i > 0 ? -1 * i : i;
  auto x = myodd::dynamic::Any( i );
  auto y = myodd::dynamic::Any(nullptr);
  ASSERT_TRUE(x < y);
  ASSERT_FALSE(x > y);
  ASSERT_FALSE(x >= y);
}

TEST_MEM(AnyTestOperators, NullEqualsAnyNul)
{
  auto x = myodd::dynamic::Any(nullptr);
  ASSERT_TRUE(x == nullptr);
  ASSERT_TRUE(nullptr == x);
}

TEST_MEM(AnyTestOperators, NullPlusNullEqualZero)
{
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(nullptr);
  auto z = x + y;
  ASSERT_EQ(0, z);
  ASSERT_EQ(myodd::dynamic::Integer_int, z.Type());
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

TEST_MEM(AnyTestOperators, AddTwoDifferentTypes)
{
  auto x = myodd::dynamic::Any((int)10);
  auto y = myodd::dynamic::Any((double)20.5);
  auto z = x + y;
  ASSERT_EQ(30.5, z);
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
  auto x = myodd::dynamic::Any(true);
  auto y = myodd::dynamic::Any(false);
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
  auto x = myodd::dynamic::Any(nullptr);
  auto y = myodd::dynamic::Any(true);
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
  auto y = myodd::dynamic::Any('9');
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
  ASSERT_EQ(8, z);
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
  ASSERT_EQ(8, z);
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

TEST_MEM(AnyTestOperators, AddACharAndAWideChar)
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
  auto nullany = myodd::dynamic::Any(nullptr);
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
  auto nullany = myodd::dynamic::Any((double)0.0);
  EXPECT_THROW(any / nullany, std::overflow_error);
}

TEST_MEM(AnyTestOperators, DivideTwoNullAnyWillThrow)
{
  auto nullany1 = myodd::dynamic::Any(nullptr);
  auto nullany2 = myodd::dynamic::Any(nullptr);
  EXPECT_THROW(nullany1 / nullany2, std::overflow_error);
}

TEST_MEM(AnyTestOperators, ZeroDividedByAnythingIsZero)
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
    if (0 == modf(d, &intpart))
    {
      continue;
    }

    auto any1 = myodd::dynamic::Any(l1);
    auto any2 = myodd::dynamic::Any(l2);

    auto anyResult = any1 / any2;
    ASSERT_DOUBLE_EQ(d, anyResult);

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
  auto any2 = myodd::dynamic::Any((long)52);

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

TEST_MEM_LOOP(AnyTestOperators, QuickAddPrefixToShort, NUMBER_OF_TESTS)
{
  auto number = IntRandomNumber<short>(false);
  auto any = number;
  ASSERT_EQ(any, number);

  ASSERT_EQ(++any, (int)number + 1);
  ASSERT_EQ(any, (int)number + 1);
}

TEST_MEM_LOOP(AnyTestOperators, QuickAddPrefixToUnsignedLongLong, NUMBER_OF_TESTS)
{
  auto number = IntRandomNumber<unsigned long long>(false);
  auto any = number;
  ASSERT_EQ(any, number);

  ASSERT_EQ(++any, number + 1);
  ASSERT_EQ(any, number + 1);
}

TEST_MEM_LOOP(AnyTestOperators, QuickSubPrefixToShort, NUMBER_OF_TESTS)
{
  auto number = IntRandomNumber<short>(false);
  auto any = myodd::dynamic::Any(number);
  ASSERT_EQ(any, number);

  ASSERT_EQ(--any, (number - 1));
  ASSERT_EQ(any, (number - 1));
}

TEST_MEM_LOOP(AnyTestOperators, QuickSubPrefixToUnsignedLongLong, NUMBER_OF_TESTS)
{
  auto number = IntRandomNumber<unsigned long long>(false);
  auto any = myodd::dynamic::Any(number);
  ASSERT_EQ(any, number);

  ASSERT_EQ(--any, (number - 1));
  ASSERT_EQ(any, (number - 1));
}

TEST_MEM_LOOP(AnyTestOperators, QuickAddPostFixToShort, NUMBER_OF_TESTS)
{
  auto number = IntRandomNumber<short>(false);
  auto any = myodd::dynamic::Any(number);
  ASSERT_EQ(any, number);

  ASSERT_EQ(any++, number);
  ASSERT_EQ(any, (number + 1));
}

TEST_MEM_LOOP(AnyTestOperators, QuickAddPostFixToUnsignedLonLong, NUMBER_OF_TESTS)
{
  auto number = IntRandomNumber<unsigned long long>(false);
  auto any = myodd::dynamic::Any(number);
  ASSERT_EQ(any, number);

  ASSERT_EQ(any++, number);
  ASSERT_EQ(any, (number + 1));
}

TEST_MEM_LOOP(AnyTestOperators, QuickSubPostFix, NUMBER_OF_TESTS)
{
  auto number = IntRandomNumber<short>(false);
  auto any = myodd::dynamic::Any(number);
  ASSERT_EQ(any, number);

  ASSERT_EQ(any--, number);
  ASSERT_EQ(any, (number - 1));
}


TEST_MEM(AnyTestOperators, CompareTwoStringsThatAreTheSame)
{
  // the two are the same
  auto any1 = myodd::dynamic::Any("+12345");
  auto any2 = myodd::dynamic::Any("12345");

  ASSERT_EQ(any1, any2);
  ASSERT_EQ(12345, any1);
  ASSERT_EQ(12345, any2);
}

TEST_MEM(AnyTestOperators, CompareTwoStringsLooksLikeDoubleThatAreTheSame)
{
  // the two are the same
  auto any1 = myodd::dynamic::Any("+12345.000");
  auto any2 = myodd::dynamic::Any("12345");

  ASSERT_EQ(any1, any2);
  ASSERT_EQ(12345, any1);
  ASSERT_EQ(12345, any2);
}

TEST_MEM(AnyTestOperators, CompareTwoWideStringsLooksLikeDoubleThatAreTheSame)
{
  // the two are the same
  auto any1 = myodd::dynamic::Any("+12345.000");
  auto any2 = myodd::dynamic::Any("12345");

  ASSERT_EQ(any1, any2);
  ASSERT_EQ(12345, any1);
  ASSERT_EQ(12345, any2);
}

TEST_MEM(AnyTestOperators, CompareTwoStringsWithDoubleThatAreTheSame)
{
  // the two are the same
  auto any1 = myodd::dynamic::Any("+12345.678");
  auto any2 = myodd::dynamic::Any("12345.678");

  ASSERT_EQ(any1, any2);
  ASSERT_EQ(12345.678, any1);
  ASSERT_EQ(12345.678, any2);
}

TEST_MEM(AnyTestOperators, CompareTwoWideStringsWithDoubleThatAreTheSame)
{
  // the two are the same
  auto any1 = myodd::dynamic::Any(L"+12345.678");
  auto any2 = myodd::dynamic::Any(L"12345.678");

  ASSERT_EQ(any1, any2);
  ASSERT_EQ(12345.678, any1);
  ASSERT_EQ(12345.678, any2);
}

TEST_MEM(AnyTestOperators, CompareTwoZeroStringsThatAreTheSame)
{
  // the three are the same
  auto any1 = myodd::dynamic::Any("+0");
  auto any2 = myodd::dynamic::Any("-0");
  auto any3 = myodd::dynamic::Any("0");

  ASSERT_EQ(any1, any2);
  ASSERT_EQ(any2, any3);
  ASSERT_EQ(any1, any3);
  ASSERT_EQ(0, any1);
  ASSERT_EQ(0, any2);
  ASSERT_EQ(0, any3);
  ASSERT_EQ(0.f, any1);
  ASSERT_EQ(0.f, any2);
}

TEST_MEM(AnyTestOperators, EmptyStringIsNotZero)
{
  // the two are the same
  auto any1 = myodd::dynamic::Any("");
  auto any2 = myodd::dynamic::Any("0");

  ASSERT_NE(any1, any2);
}

TEST_MEM(AnyTestOperators, EmptyWideStringIsNotZero)
{
  // the two are the same
  auto any1 = myodd::dynamic::Any(L"");
  auto any2 = myodd::dynamic::Any(L"0");

  ASSERT_NE(any1, any2);
}

TEST_MEM(AnyTestOperators, SingleCharIsANumber)
{
  // the two are the same
  auto any1 = myodd::dynamic::Any('1');
  auto any2 = myodd::dynamic::Any("+1");

  ASSERT_EQ(any1, any2);
}

TEST_MEM(AnyTestOperators, SingleWideCharIsANumber)
{
  // the two are the same
  auto any1 = myodd::dynamic::Any(L'1');
  auto any2 = myodd::dynamic::Any(L"+1");

  ASSERT_EQ(any1, any2);
}


TEST_MEM(AnyTestOperators, CompareTwoZeroWideStringsThatAreTheSame)
{
  // the three are the same
  auto any1 = myodd::dynamic::Any(L"+0");
  auto any2 = myodd::dynamic::Any(L"-0");
  auto any3 = myodd::dynamic::Any(L"0");

  ASSERT_EQ(any1, any2);
  ASSERT_EQ(any2, any3);
  ASSERT_EQ(any1, any3);
  ASSERT_EQ(0, any1);
  ASSERT_EQ(0, any2);
  ASSERT_EQ(0, any3);
  ASSERT_EQ(0.f, any1);
  ASSERT_EQ(0.f, any2);
}
///////////////////////////////////////////////////////////////

TEST_MEM_LOOP(AnyTestOperators, AddShortIntToAny, NUMBER_OF_TESTS)
{
  auto start = IntRandomNumber<short int>(false);
  auto number = IntRandomNumber<short int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any += number;
  ASSERT_EQ(any, (start + number));

  any = myodd::dynamic::Any(start);
  any = any + number;
  ASSERT_EQ(any, (start + number));
}

TEST_MEM_LOOP(AnyTestOperators, AddUnsignedShortIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, AddIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, AddUnsignedIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, AddLongIntToAny, NUMBER_OF_TESTS)
{
  auto start = IntRandomNumber <long int >(false);
  auto number = IntRandomNumber<long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any += number;
  ASSERT_EQ(any, (start + number));

  any = myodd::dynamic::Any(start);
  any = any + number;
  ASSERT_EQ(any, (start + number));
}

TEST_MEM_LOOP(AnyTestOperators, AddUnsignedLongIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, AddLongLongIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, AddUnsignedLongLongIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, AddFloatToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, AddDoubleToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, AddLongDoubleToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, SubstractShortIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, SubstractUnsignedShortIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, SubstractIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, SubstractUnsignedIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, SubstractLongIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, SubstractUnsignedLongIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, SubstractLongLongIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, SubstractUnsignedLongLongIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, MultiplyShortIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, MultiplyUnsignedShortIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, MultiplyIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, MultiplyUnsignedIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, MultiplyLongIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, MultiplyUnsignedLongIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, MultiplyLongLongIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, MultiplyUnsignedLongLongIntToAny, NUMBER_OF_TESTS)
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

TEST_MEM_LOOP(AnyTestOperators, DivideShortIntToAny, NUMBER_OF_TESTS)
{
  auto start = IntRandomNumber<short int>(false);
  auto number = IntRandomNumber<short int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any /= number;
  ASSERT_DOUBLE_EQ(any, ((double)start / number));

  any = myodd::dynamic::Any(start);
  any = any / number;
  ASSERT_DOUBLE_EQ(any, ((double)start / number));
}

TEST_MEM_LOOP(AnyTestOperators, DivideUnsignedShortIntToAny, NUMBER_OF_TESTS)
{
  auto start = IntRandomNumber<unsigned short int>(false);
  auto number = IntRandomNumber<unsigned short int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any /= number;
  ASSERT_DOUBLE_EQ(any, ((double)start / number));

  any = myodd::dynamic::Any(start);
  any = any / number;
  ASSERT_DOUBLE_EQ(any, ((double)start / number));
}

TEST_MEM_LOOP(AnyTestOperators, DivideIntToAny, NUMBER_OF_TESTS)
{
  auto start = IntRandomNumber<int>(false);
  auto number = IntRandomNumber<int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any /= number;
  ASSERT_DOUBLE_EQ(any, ((double)start / number));

  any = myodd::dynamic::Any(start);
  any = any / number;
  ASSERT_DOUBLE_EQ(any, ((double)start / number));
}

TEST_MEM_LOOP(AnyTestOperators, DivideUnsignedIntToAny, NUMBER_OF_TESTS)
{
  auto start = IntRandomNumber<unsigned int>(false);
  auto number = IntRandomNumber<unsigned int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any /= number;
  ASSERT_DOUBLE_EQ(any, ((double)start / number));

  any = myodd::dynamic::Any(start);
  any = any / number;
  ASSERT_DOUBLE_EQ(any, ((double)start / number));
}

TEST_MEM_LOOP(AnyTestOperators, DivideLongIntToAny, NUMBER_OF_TESTS)
{
  auto start = IntRandomNumber <long int >(false);
  auto number = IntRandomNumber<long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any /= number;
  ASSERT_DOUBLE_EQ(any, ((double)start / number));

  any = myodd::dynamic::Any(start);
  any = any / number;
  ASSERT_DOUBLE_EQ(any, ((double)start / number));
}

TEST_MEM_LOOP(AnyTestOperators, DivideUnsignedLongIntToAny, NUMBER_OF_TESTS)
{
  auto start = IntRandomNumber <unsigned long int >(false);
  auto number = IntRandomNumber<unsigned long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any /= number;
  ASSERT_EQ(any, ((long double)start / number));

  any = myodd::dynamic::Any(start);
  any = any / number;
  ASSERT_EQ(any, ((long double)start / number));
}

TEST_MEM_LOOP(AnyTestOperators, DivideLongLongIntToAny, NUMBER_OF_TESTS)
{
  auto start = IntRandomNumber <long long int >(false);
  auto number = IntRandomNumber<long long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any /= number;
  ASSERT_EQ(any, ((long double)start / number));

  any = myodd::dynamic::Any(start);
  any = any / number;
  ASSERT_EQ(any, ((long double)start / number));
}

TEST_MEM(AnyTestOperators, DivideUnsignedLongLongIntToAny)
{
  auto start = IntRandomNumber<unsigned long long int>(false);
  auto number = IntRandomNumber<unsigned long long int>(false);
  auto any = myodd::dynamic::Any(start);
  ASSERT_EQ(any, start);
  any /= number;
  ASSERT_EQ(any, ((long double)start / (long double)number));

  any = myodd::dynamic::Any(start);
  any = any / number;
  ASSERT_EQ(any, (long double)((long double)start / (long double)number));
}

TEST_MEM(AnyTestOperators, DivideLargeUnsignedLongLongIntToAny)
{
  {
    auto start = (unsigned long long int) 15023239872001903631;
    auto number = (unsigned long long int)1712482809020384196;
    auto any = myodd::dynamic::Any(start);
    ASSERT_EQ(any, start);
    any /= number;
    ASSERT_EQ(any, ((long double)start / (long double)number));

    any = myodd::dynamic::Any(start);
    any = any / number;
    ASSERT_EQ(any, (long double)((long double)start / (long double)number));
  }
  {
    auto start = (unsigned long long int) 5006348524501361372;
    auto number = (unsigned long long int) 17417882237529564029;
    auto any = myodd::dynamic::Any(start);
    ASSERT_EQ(any, start);
    any /= number;
    ASSERT_EQ(any, ((long double)start / (long double)number));

    any = myodd::dynamic::Any(start);
    any = any / number;
    ASSERT_EQ(any, (long double)((long double)start / (long double)number));
  }
}

TEST_MEM(AnyTestOperators, AddLargeUnsignedLongLongIntToAny)
{
  {
    auto start = (unsigned long long int) 15023239872001903631;
    auto number = (unsigned long long int)1712482809020384196;
    auto any = myodd::dynamic::Any(start);
    ASSERT_EQ(any, start);
    any += number;
    ASSERT_EQ(any, start + number);

    any = myodd::dynamic::Any(start);
    any = any + number;
    ASSERT_EQ(any, start + number);
  }
  {
    auto start = (unsigned long long int) 5006348524501361372;
    auto number = (unsigned long long int) 17417882237529564029;
    auto any = myodd::dynamic::Any(start);
    ASSERT_EQ(any, start);
    any += number;
    ASSERT_EQ(any, start + number);

    any = myodd::dynamic::Any(start);
    any = any + number;
    ASSERT_EQ(any, start + number);
  }
}

TEST_MEM(AnyTestOperators, SubstractLargeUnsignedLongLongIntToAny)
{
  {
    auto start = (unsigned long long int) 15023239872001903631;
    auto number = (unsigned long long int)1712482809020384196;
    auto any = myodd::dynamic::Any(start);
    ASSERT_EQ(any, start);
    any -= number;
    ASSERT_EQ(any, start - number);

    any = myodd::dynamic::Any(start);
    any = any - number;
    ASSERT_EQ(any, start - number);
  }
  {
    auto start = (unsigned long long int) 5006348524501361372;
    auto number = (unsigned long long int) 17417882237529564029;
    auto any = myodd::dynamic::Any(start);
    ASSERT_EQ(any, start);
    any -= number;
    ASSERT_EQ(any, start - number);

    any = myodd::dynamic::Any(start);
    any = any - number;
    ASSERT_EQ(any, start - number);
  }
}

TEST_MEM(AnyTestOperators, MultiplyLargeUnsignedLongLongIntToAny)
{
  {
    auto start = (unsigned long long int) 15023239872001903631;
    auto number = (unsigned long long int)1712482809020384196;
    auto any = myodd::dynamic::Any(start);
    ASSERT_EQ(any, start);
    any *= number;
    ASSERT_EQ(any, start * number);

    any = myodd::dynamic::Any(start);
    any = any * number;
    ASSERT_EQ(any, start * number);
  }
  {
    auto start = (unsigned long long int) 5006348524501361372;
    auto number = (unsigned long long int) 17417882237529564029;
    auto any = myodd::dynamic::Any(start);
    ASSERT_EQ(any, start);
    any *= number;
    ASSERT_EQ(any, start * number);

    any = myodd::dynamic::Any(start);
    any = any * number;
    ASSERT_EQ(any, start * number);
  }
}

TEST_MEM(AnyTestOperators, DivideLargeUnsignedLongLongIntAsStringToAny)
{
  {
    auto start = (unsigned long long int) 15023239872001903631;
    auto number = (unsigned long long int)1712482809020384196;
    auto any = myodd::dynamic::Any("15023239872001903631");
    ASSERT_EQ(any, start);
    any /= "1712482809020384196";
    ASSERT_EQ(any, ((long double)start / (long double)number));

    any = myodd::dynamic::Any("15023239872001903631");
    any = any / "1712482809020384196";
    ASSERT_EQ(any, (long double)((long double)start / (long double)number));
  }
  {
    auto start = (unsigned long long int) 5006348524501361372;
    auto number = (unsigned long long int) 17417882237529564029;
    auto any = myodd::dynamic::Any("5006348524501361372");
    ASSERT_EQ(any, start);
    any /= "17417882237529564029";
    ASSERT_EQ(any, ((long double)start / (long double)number));

    any = myodd::dynamic::Any("5006348524501361372");
    any = any / "17417882237529564029";
    ASSERT_EQ(any, (long double)((long double)start / (long double)number));
  }
}

TEST_MEM(AnyTestOperators, DivideLargeUnsignedLongLongIntAsWideStringToAny)
{
  {
    auto start = (unsigned long long int) 15023239872001903631;
    auto number = (unsigned long long int)1712482809020384196;
    auto any = myodd::dynamic::Any(L"15023239872001903631");
    ASSERT_EQ(any, start);
    any /= L"1712482809020384196";
    ASSERT_EQ(any, ((long double)start / (long double)number));

    any = myodd::dynamic::Any(L"15023239872001903631");
    any = any / L"1712482809020384196";
    ASSERT_EQ(any, (long double)((long double)start / (long double)number));
  }
  {
    auto start = (unsigned long long int) 5006348524501361372;
    auto number = (unsigned long long int) 17417882237529564029;
    auto any = myodd::dynamic::Any(L"5006348524501361372");
    ASSERT_EQ(any, start);
    any /= L"17417882237529564029";
    ASSERT_EQ(any, ((long double)start / (long double)number));

    any = myodd::dynamic::Any(L"5006348524501361372");
    any = any / L"17417882237529564029";
    ASSERT_EQ(any, (long double)((long double)start / (long double)number));
  }
}

TEST_MEM(AnyTestOperators, LongDoubleStringMultiplied)
{
  {
    myodd::dynamic::Any pi = "3.14159265358979323846264338327950288419716939937510";
    auto pi2 = pi * "2";
    ASSERT_EQ(pi2, "6.2831853071795864769252867665590057683943387987502");
    ASSERT_EQ(pi2, 6.2831853071795864769252867665590057683943387987502L);
  }
  {
    myodd::dynamic::Any two = "2";
    auto pi2 = two * "3.14159265358979323846264338327950288419716939937510";
    ASSERT_EQ(pi2, "6.2831853071795864769252867665590057683943387987502");
    ASSERT_EQ(pi2, 6.2831853071795864769252867665590057683943387987502L);
  }
}

TEST_MEM_LOOP(AnyTestOperators, CompareTwoFloats, NUMBER_OF_TESTS )
{
  for (;;)
  {
    float number = RealRandomNumber<float>(false);
    if (number == (int)number)
    {
      continue;
    }
    auto any = ::myodd::dynamic::Any(number);
    ASSERT_NE((long long)number, any);
    ASSERT_EQ(number, any);

    // done
    break;
  }
}

TEST_MEM_LOOP(AnyTestOperators, CompareTwoDouble, NUMBER_OF_TESTS)
{
  for (;;)
  {
    auto number = RealRandomNumber<double>(false);
    if (number == (int)number)
    {
      continue;
    }
    auto any = ::myodd::dynamic::Any(number);
    ASSERT_NE((int)number, any);
    ASSERT_NE((long)number, any);
    ASSERT_EQ(number, any);

    // done
    break;
  }
}

TEST_MEM(AnyTestOperators, CheckMemoryLeakAsignAShortInt)
{
  // make it a string
  auto any = ::myodd::dynamic::Any("Hello");

  // give it another value
  any = IntRandomNumber<short int>();
  
  // memory of the string should have been cleared.
}

TEST_MEM(AnyTestOperators, CheckMemoryLeakAsignAnUnsignedShortInt)
{
  // make it a string
  auto any = ::myodd::dynamic::Any("Hello");

  // give it another value
  any = IntRandomNumber<unsigned short int>();

  // memory of the string should have been cleared.
}

TEST_MEM(AnyTestOperators, CheckMemoryLeakAsignInt)
{
  // make it a string
  auto any = ::myodd::dynamic::Any("Hello");

  // give it another value
  any = IntRandomNumber<int>();

  // memory of the string should have been cleared.
}

TEST_MEM(AnyTestOperators, CheckMemoryLeakAsignAnUnsignedInt)
{
  // make it a string
  auto any = ::myodd::dynamic::Any("Hello");

  // give it another value
  any = IntRandomNumber<unsigned int>();

  // memory of the string should have been cleared.
}

TEST_MEM(AnyTestOperators, CheckMemoryLeakAsignALong)
{
  // make it a string
  auto any = ::myodd::dynamic::Any("Hello");

  // give it another value
  any = IntRandomNumber<long int>();

  // memory of the string should have been cleared.
}

TEST_MEM(AnyTestOperators, CheckMemoryLeakAsignAnUnsignedLong)
{
  // make it a string
  auto any = ::myodd::dynamic::Any("Hello");

  // give it another value
  any = IntRandomNumber<unsigned long int>();

  // memory of the string should have been cleared.
}

TEST_MEM(AnyTestOperators, CheckMemoryLeakAsignALongLong)
{
  // make it a string
  auto any = ::myodd::dynamic::Any("Hello");

  // give it another value
  any = IntRandomNumber<long long int>();

  // memory of the string should have been cleared.
}

TEST_MEM(AnyTestOperators, CheckMemoryLeakAsignAnUnsignedLongLong)
{
  // make it a string
  auto any = ::myodd::dynamic::Any("Hello");

  // give it another value
  any = IntRandomNumber<unsigned long long int>();

  // memory of the string should have been cleared.
}

TEST_MEM(AnyTestOperators, CheckMemoryLeakAsignAFloat)
{
  // make it a string
  auto any = ::myodd::dynamic::Any("Hello");

  // give it another value
  any = RealRandomNumber<float>();

  // memory of the string should have been cleared.
}

TEST_MEM(AnyTestOperators, CheckMemoryLeakAsignADouble)
{
  // make it a string
  auto any = ::myodd::dynamic::Any("Hello");

  // give it another value
  any = RealRandomNumber<double>();

  // memory of the string should have been cleared.
}

TEST_MEM(AnyTestOperators, CheckMemoryLeakAsignALongDouble)
{
  // make it a string
  auto any = ::myodd::dynamic::Any("Hello");

  // give it another value
  any = RealRandomNumber<long double>();

  // memory of the string should have been cleared.
}

TEST_MEM(AnyTestOperators, CheckMemoryLeakAsignANull)
{
  // make it a string
  auto any = ::myodd::dynamic::Any("Hello");

  // give it another value
  any = nullptr;

  // memory of the string should have been cleared.
}

TEST_MEM(AnyTestOperators, CheckMemoryLeakAsignAnotherString)
{
  // make it a string
  auto any = ::myodd::dynamic::Any("Hello");

  // give it another value
  any = L"World";

  // memory of the string should have been cleared.
}

TEST_MEM(AnyTestOperators, CheckMemoryLeakAsignABool)
{
  // make it a string
  auto any = ::myodd::dynamic::Any("Hello");

  // give it another value
  any = true;

  // memory of the string should have been cleared.
}

TEST_MEM(AnyTestOperators, CheckLogicalNegationOperatorWithTrue)
{
  // this is true
  auto any = ::myodd::dynamic::Any(true);
  ASSERT_FALSE(!any);
  
  // we can't just use ASSERT_TRUE() as Google needs to know 
  // what to cast any to, by default it will try and cast it
  // to const AssertionResult&
  ASSERT_EQ(true, any);
}

TEST_MEM(AnyTestOperators, CheckLogicalNegationOperatorWithFalse)
{
  // this is true
  auto any = ::myodd::dynamic::Any(false);
  ASSERT_TRUE(!any);
  ASSERT_TRUE(!(any));

  // we can't just use ASSERT_FALSE() as it translates to !(any)
  ASSERT_EQ( false, any );
}

TEST_MEM_LOOP(AnyTestOperators, CompareTwoIntegerNumbersRandom, NUMBER_OF_TESTS )
{
  // get 2 numbers
  auto x = IntRandomNumber<int>();
  auto y = IntRandomNumber<int>();

  auto anyx = ::myodd::dynamic::Any(x);
  auto anyy = ::myodd::dynamic::Any(y);

  if (x > y)
  {
    ASSERT_TRUE(anyx > anyy);
    ASSERT_TRUE(anyx > y );
    ASSERT_TRUE(x > anyy);
    ASSERT_TRUE(anyx >= anyy);
   
    ASSERT_FALSE(anyx <= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx < anyy);
    ASSERT_FALSE(anyx < y);
  }

  if (x < y)
  {
    ASSERT_TRUE(anyx < anyy);
    ASSERT_TRUE(anyx < y);
    ASSERT_TRUE(x < anyy);
    ASSERT_TRUE(anyx <= anyy);

    ASSERT_FALSE(anyx >= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }

  if (x == y)
  {
    ASSERT_TRUE(anyx == anyy);
    ASSERT_TRUE(anyx <= anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx < y);
    ASSERT_FALSE(x < anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }
}

TEST_MEM_LOOP(AnyTestOperators, CompareTwoUnsignedLongLongIntNumbersRandom, NUMBER_OF_TESTS)
{
  // get 2 numbers
  auto x = IntRandomNumber<unsigned long long int>();
  auto y = IntRandomNumber<unsigned long long int>();

  auto anyx = ::myodd::dynamic::Any(x);
  auto anyy = ::myodd::dynamic::Any(y);

  if (x > y)
  {
    ASSERT_TRUE(anyx > anyy);
    ASSERT_TRUE(anyx > y);
    ASSERT_TRUE(x > anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx <= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx < anyy);
    ASSERT_FALSE(anyx < y);
  }

  if (x < y)
  {
    ASSERT_TRUE(anyx < anyy);
    ASSERT_TRUE(anyx < y);
    ASSERT_TRUE(x < anyy);
    ASSERT_TRUE(anyx <= anyy);

    ASSERT_FALSE(anyx >= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }

  if (x == y)
  {
    ASSERT_TRUE(anyx == anyy);
    ASSERT_TRUE(anyx <= anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx < y);
    ASSERT_FALSE(x < anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }
}

TEST_MEM_LOOP(AnyTestOperators, CompareTwoUnsignedLongLongIntAndLongLongIntNumbersRandom, NUMBER_OF_TESTS)
{
  // get 2 numbers
  auto x = IntRandomNumber<unsigned long long int>();
  auto y = IntRandomNumber<long long int>();

  auto anyx = ::myodd::dynamic::Any(x);
  auto anyy = ::myodd::dynamic::Any(y);

  if ( y > 0 && x > (unsigned long long int)y)
  {
    ASSERT_TRUE(anyx > anyy);
    ASSERT_TRUE(anyx > y);
    ASSERT_TRUE(x > anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_TRUE(anyy < anyx);
    ASSERT_TRUE(anyy < x);
    ASSERT_TRUE(y < anyx);
    ASSERT_TRUE(anyy <= anyx);

    ASSERT_FALSE(anyx <= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx < anyy);
    ASSERT_FALSE(anyx < y);
  }

  if (y < 0 || (unsigned long long int)y < x )
  {
    ASSERT_TRUE(anyy < anyx);
    ASSERT_TRUE(anyy < x);
    ASSERT_TRUE(y < anyx);
    ASSERT_TRUE(anyy <= anyx);

    ASSERT_FALSE(anyy >= anyx);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyy > anyx);
    ASSERT_FALSE(y > anyx);
  }

  if (x == y)
  {
    ASSERT_TRUE(anyx == anyy);
    ASSERT_TRUE(anyx <= anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx < y);
    ASSERT_FALSE(x < anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }
}

TEST_MEM_LOOP(AnyTestOperators, CompareTwoLongLongIntAndUnsignedLongLongIntNumbersRandom, NUMBER_OF_TESTS)
{
  // get 2 numbers
  auto x = IntRandomNumber<long long int>();
  auto y = IntRandomNumber<unsigned long long int>();

  auto anyx = ::myodd::dynamic::Any(x);
  auto anyy = ::myodd::dynamic::Any(y);

  if (x > 0 && (unsigned long long int)x > y )
  {
    ASSERT_TRUE(anyx > anyy);
    ASSERT_TRUE(anyx > y);
    ASSERT_TRUE(x > anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_TRUE(anyy < anyx);
    ASSERT_TRUE(anyy < x);
    ASSERT_TRUE(y < anyx);
    ASSERT_TRUE(anyy <= anyx);

    ASSERT_FALSE(anyx <= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx < anyy);
    ASSERT_FALSE(anyx < y);
  }

  if (x < 0 || (unsigned long long int)x < y)
  {
    ASSERT_TRUE(anyx < anyy);
    ASSERT_TRUE(anyx < y);
    ASSERT_TRUE(x < anyy);
    ASSERT_TRUE(anyx <= anyy);

    ASSERT_FALSE(anyx >= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(x > anyy);
  }

  if (x == y)
  {
    ASSERT_TRUE(anyx == anyy);
    ASSERT_TRUE(anyx <= anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx < y);
    ASSERT_FALSE(x < anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }
}

TEST_MEM_LOOP(AnyTestOperators, CompareTwoShortNumbersRandom, NUMBER_OF_TESTS)
{
  // get 2 numbers
  auto x = IntRandomNumber<short>();
  auto y = IntRandomNumber<short>();

  auto anyx = ::myodd::dynamic::Any(x);
  auto anyy = ::myodd::dynamic::Any(y);

  if (x > y)
  {
    ASSERT_TRUE(anyx > anyy);
    ASSERT_TRUE(anyx > y);
    ASSERT_TRUE(x > anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx <= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx < anyy);
    ASSERT_FALSE(anyx < y);
  }

  if (x < y)
  {
    ASSERT_TRUE(anyx < anyy);
    ASSERT_TRUE(anyx < y);
    ASSERT_TRUE(x < anyy);
    ASSERT_TRUE(anyx <= anyy);

    ASSERT_FALSE(anyx >= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }

  if (x == y)
  {
    ASSERT_TRUE(anyx == anyy);
    ASSERT_TRUE(anyx <= anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx < y);
    ASSERT_FALSE(x < anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }
}

TEST_MEM_LOOP(AnyTestOperators, CompareTwoDoubleNumbersRandom, NUMBER_OF_TESTS)
{
  // get 2 numbers
  auto x = RealRandomNumber<double>();
  auto y = RealRandomNumber<double>();

  auto anyx = ::myodd::dynamic::Any(x);
  auto anyy = ::myodd::dynamic::Any(y);

  if (x > y)
  {
    ASSERT_TRUE(anyx > anyy);
    ASSERT_TRUE(anyx > y);
    ASSERT_TRUE(x > anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx <= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx < anyy);
    ASSERT_FALSE(anyx < y);
  }

  if (x < y)
  {
    ASSERT_TRUE(anyx < anyy);
    ASSERT_TRUE(anyx < y);
    ASSERT_TRUE(x < anyy);
    ASSERT_TRUE(anyx <= anyy);

    ASSERT_FALSE(anyx >= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }

  if (x == y)
  {
    ASSERT_TRUE(anyx == anyy);
    ASSERT_TRUE(anyx <= anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx < y);
    ASSERT_FALSE(x < anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }
}

TEST_MEM_LOOP(AnyTestOperators, CompareTwoFloatNumbersRandom, NUMBER_OF_TESTS)
{
  // get 2 numbers
  auto x = RealRandomNumber<float>();
  auto y = RealRandomNumber<float>();

  auto anyx = ::myodd::dynamic::Any(x);
  auto anyy = ::myodd::dynamic::Any(y);

  if (x > y)
  {
    ASSERT_TRUE(anyx > anyy);
    ASSERT_TRUE(anyx > y);
    ASSERT_TRUE(x > anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx <= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx < anyy);
    ASSERT_FALSE(anyx < y);
  }

  if (x < y)
  {
    ASSERT_TRUE(anyx < anyy);
    ASSERT_TRUE(anyx < y);
    ASSERT_TRUE(x < anyy);
    ASSERT_TRUE(anyx <= anyy);

    ASSERT_FALSE(anyx >= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }

  if (x == y)
  {
    ASSERT_TRUE(anyx == anyy);
    ASSERT_TRUE(anyx <= anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx < y);
    ASSERT_FALSE(x < anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }
}

TEST_MEM_LOOP(AnyTestOperators, CompareTwoUnsignedLongLongIntAndDoubleNumbersRandom, NUMBER_OF_TESTS)
{
  // get 2 numbers
  auto x = IntRandomNumber<unsigned long long int>();
  auto y = RealRandomNumber<double>();

  auto anyx = ::myodd::dynamic::Any(x);
  auto anyy = ::myodd::dynamic::Any(y);

  if (y > 0 && x > y)
  {
    ASSERT_TRUE(anyx > anyy);
    ASSERT_TRUE(anyx > y);
    ASSERT_TRUE(x > anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_TRUE(anyy < anyx);
    ASSERT_TRUE(anyy < x);
    ASSERT_TRUE(y < anyx);
    ASSERT_TRUE(anyy <= anyx);

    ASSERT_FALSE(anyx <= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx < anyy);
    ASSERT_FALSE(anyx < y);
  }

  if (y < 0 || y < x)
  {
    ASSERT_TRUE(anyy < anyx);
    ASSERT_TRUE(anyy < x);
    ASSERT_TRUE(y < anyx);
    ASSERT_TRUE(anyy <= anyx);

    ASSERT_FALSE(anyy >= anyx);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyy > anyx);
    ASSERT_FALSE(y > anyx);
  }

  if (x == y)
  {
    ASSERT_TRUE(anyx == anyy);
    ASSERT_TRUE(anyx <= anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx < y);
    ASSERT_FALSE(x < anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }
}

TEST_MEM_LOOP(AnyTestOperators, CompareTwoUnsignedShortIntAndDoubleNumbersRandom, NUMBER_OF_TESTS)
{
  // get 2 numbers
  auto x = IntRandomNumber<unsigned short int>();
  auto y = RealRandomNumber<double>();

  auto anyx = ::myodd::dynamic::Any(x);
  auto anyy = ::myodd::dynamic::Any(y);

  if (y > 0 && x > y)
  {
    ASSERT_TRUE(anyx > anyy);
    ASSERT_TRUE(anyx > y);
    ASSERT_TRUE(x > anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_TRUE(anyy < anyx);
    ASSERT_TRUE(anyy < x);
    ASSERT_TRUE(y < anyx);
    ASSERT_TRUE(anyy <= anyx);

    ASSERT_FALSE(anyx <= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx < anyy);
    ASSERT_FALSE(anyx < y);
  }

  if (y < 0 || y < x)
  {
    ASSERT_TRUE(anyy < anyx);
    ASSERT_TRUE(anyy < x);
    ASSERT_TRUE(y < anyx);
    ASSERT_TRUE(anyy <= anyx);

    ASSERT_FALSE(anyy >= anyx);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyy > anyx);
    ASSERT_FALSE(y > anyx);
  }

  if (x == y)
  {
    ASSERT_TRUE(anyx == anyy);
    ASSERT_TRUE(anyx <= anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx < y);
    ASSERT_FALSE(x < anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }
}

TEST_MEM_LOOP(AnyTestOperators, CompareWideStringsToNumber, NUMBER_OF_TESTS )
{
  const size_t len = 10;
  wchar_t wc[len];
  long long int bignum = 0;

  for (auto i = 0; i < len-1; ++i)
  {
    auto num = IntRandomNumber<unsigned short>(0, 9, false);
    wc[i] = L'0' + num;
    bignum = (bignum * 10) + num;
  }
  wc[len-1] = L'\0';

  auto any = ::myodd::dynamic::Any(wc);
  ASSERT_TRUE(any == bignum);
  ASSERT_TRUE(any >= bignum);
  ASSERT_TRUE(any <= bignum);
  ASSERT_TRUE(any > bignum-1);
  ASSERT_TRUE(any < bignum+1);

  ASSERT_TRUE(bignum == any);
  ASSERT_TRUE(bignum >= any);
  ASSERT_TRUE(bignum <= any);
  ASSERT_TRUE(bignum+1 > any);
  ASSERT_TRUE(bignum-1 < any);
}

TEST_MEM_LOOP(AnyTestOperators, CompareStringsToNumber, NUMBER_OF_TESTS)
{
  const size_t len = 10;
  char c[len];
  long long int bignum = 0;

  for (auto i = 0; i < len - 1; ++i)
  {
    auto num = IntRandomNumber<unsigned short>(0, 9, false);
    c[i] = '0' + num;
    bignum = (bignum * 10) + num;
  }
  c[len - 1] = '\0';

  auto any = ::myodd::dynamic::Any(c);
  ASSERT_TRUE(any == bignum);
  ASSERT_TRUE(any >= bignum);
  ASSERT_TRUE(any <= bignum);
  ASSERT_TRUE(any > bignum - 1);
  ASSERT_TRUE(any < bignum + 1);

  ASSERT_TRUE(bignum == any);
  ASSERT_TRUE(bignum >= any);
  ASSERT_TRUE(bignum <= any);
  ASSERT_TRUE(bignum + 1 > any);
  ASSERT_TRUE(bignum - 1 < any);
}

TEST_MEM_LOOP(AnyTestOperators, NumberDoesNotEqualString, NUMBER_OF_TESTS)
{
  auto x = myodd::dynamic::Any(IntRandomNumber<int>(false) );
  auto y = myodd::dynamic::Any("Hello");
  ASSERT_FALSE(x == y );
  ASSERT_TRUE(x != y);
}

TEST_MEM_LOOP(AnyTestOperators, NumberDoesNotEqualWideString, NUMBER_OF_TESTS )
{
  auto x = myodd::dynamic::Any(IntRandomNumber<int>(false));
  auto y = myodd::dynamic::Any(L"Hello");
  ASSERT_FALSE(x == y);
  ASSERT_TRUE(x != y);
}

TEST_MEM(AnyTestOperators, ZeroDoesEqualString)
{
  auto x = myodd::dynamic::Any(0);
  auto y = myodd::dynamic::Any("Hello");
  ASSERT_FALSE(x != y);
  ASSERT_TRUE(x == y);
}

TEST_MEM(AnyTestOperators, ZeroDoesEqualWideString)
{
  auto x = myodd::dynamic::Any(0);
  auto y = myodd::dynamic::Any(L"Hello");
  ASSERT_FALSE(x != y);
  ASSERT_TRUE(x == y);
}

TEST_MEM_LOOP(AnyTestOperators, CompareTwoIntAndUnsignedIntNumbersRandom, NUMBER_OF_TESTS)
{
  // get 2 numbers
  auto x = IntRandomNumber<int>();
  auto y = IntRandomNumber<unsigned int>();

  auto anyx = ::myodd::dynamic::Any(x);
  auto anyy = ::myodd::dynamic::Any(y);

  if (x > 0 && (unsigned int)x > y)
  {
    ASSERT_TRUE(anyx > anyy);
    ASSERT_TRUE(anyx > y);
    ASSERT_TRUE(x > anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_TRUE(anyy < anyx);
    ASSERT_TRUE(anyy < x);
    ASSERT_TRUE(y < anyx);
    ASSERT_TRUE(anyy <= anyx);

    ASSERT_FALSE(anyx <= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx < anyy);
    ASSERT_FALSE(anyx < y);
  }

  if (x < 0 || (unsigned int)x < y)
  {
    ASSERT_TRUE(anyx < anyy);
    ASSERT_TRUE(anyx < y);
    ASSERT_TRUE(x < anyy);
    ASSERT_TRUE(anyx <= anyy);

    ASSERT_FALSE(anyx >= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(x > anyy);
  }

  if (x == y)
  {
    ASSERT_TRUE(anyx == anyy);
    ASSERT_TRUE(anyx <= anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx < y);
    ASSERT_FALSE(x < anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }
}

TEST_MEM_LOOP(AnyTestOperators, CompareTwoShortAndUnsignedShortNumbersRandom, NUMBER_OF_TESTS)
{
  // get 2 numbers
  auto x = IntRandomNumber<short>();
  auto y = IntRandomNumber<unsigned short>();

  auto anyx = ::myodd::dynamic::Any(x);
  auto anyy = ::myodd::dynamic::Any(y);

  if (x > 0 && (unsigned short)x > y)
  {
    ASSERT_TRUE(anyx > anyy);
    ASSERT_TRUE(anyx > y);
    ASSERT_TRUE(x > anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_TRUE(anyy < anyx);
    ASSERT_TRUE(anyy < x);
    ASSERT_TRUE(y < anyx);
    ASSERT_TRUE(anyy <= anyx);

    ASSERT_FALSE(anyx <= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx < anyy);
    ASSERT_FALSE(anyx < y);
  }

  if (x < 0 || (unsigned short)x < y)
  {
    ASSERT_TRUE(anyx < anyy);
    ASSERT_TRUE(anyx < y);
    ASSERT_TRUE(x < anyy);
    ASSERT_TRUE(anyx <= anyy);

    ASSERT_FALSE(anyx >= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(x > anyy);
  }

  if (x == y)
  {
    ASSERT_TRUE(anyx == anyy);
    ASSERT_TRUE(anyx <= anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx < y);
    ASSERT_FALSE(x < anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }
}

TEST_MEM_LOOP(AnyTestOperators, CompareTwoUnsignedShortAndSignedShortNumbersRandom, NUMBER_OF_TESTS)
{
  // get 2 numbers
  auto x = IntRandomNumber<unsigned short>();
  auto y = IntRandomNumber<short>();

  auto anyx = ::myodd::dynamic::Any(x);
  auto anyy = ::myodd::dynamic::Any(y);

  if (y > 0 && x > (unsigned short)y)
  {
    ASSERT_TRUE(anyx > anyy);
    ASSERT_TRUE(anyx > y);
    ASSERT_TRUE(x > anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_TRUE(anyy < anyx);
    ASSERT_TRUE(anyy < x);
    ASSERT_TRUE(y < anyx);
    ASSERT_TRUE(anyy <= anyx);

    ASSERT_FALSE(anyx <= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx < anyy);
    ASSERT_FALSE(anyx < y);
  }

  if (y < 0 || x < (unsigned short)y)
  {
    ASSERT_TRUE(anyx < anyy);
    ASSERT_TRUE(anyx < y);
    ASSERT_TRUE(x < anyy);
    ASSERT_TRUE(anyx <= anyy);

    ASSERT_FALSE(anyx >= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(x > anyy);
  }

  if (x == y)
  {
    ASSERT_TRUE(anyx == anyy);
    ASSERT_TRUE(anyx <= anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx < y);
    ASSERT_FALSE(x < anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }
}

TEST_MEM_LOOP(AnyTestOperators, CompareTwoUnsignedIntAndSignedIntNumbersRandom, NUMBER_OF_TESTS)
{
  // get 2 numbers
  auto x = IntRandomNumber<unsigned int>();
  auto y = IntRandomNumber<int>();

  auto anyx = ::myodd::dynamic::Any(x);
  auto anyy = ::myodd::dynamic::Any(y);

  if (y > 0 && x > (unsigned int)y)
  {
    ASSERT_TRUE(anyx > anyy);
    ASSERT_TRUE(anyx > y);
    ASSERT_TRUE(x > anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_TRUE(anyy < anyx);
    ASSERT_TRUE(anyy < x);
    ASSERT_TRUE(y < anyx);
    ASSERT_TRUE(anyy <= anyx);

    ASSERT_FALSE(anyx <= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx < anyy);
    ASSERT_FALSE(anyx < y);
  }

  if (y < 0 || x < (unsigned int)y)
  {
    ASSERT_TRUE(anyx < anyy);
    ASSERT_TRUE(anyx < y);
    ASSERT_TRUE(x < anyy);
    ASSERT_TRUE(anyx <= anyy);

    ASSERT_FALSE(anyx >= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(x > anyy);
  }

  if (x == y)
  {
    ASSERT_TRUE(anyx == anyy);
    ASSERT_TRUE(anyx <= anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx < y);
    ASSERT_FALSE(x < anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }
}

TEST_MEM_LOOP(AnyTestOperators, CompareTwoUnsignedLongIntAndSignedLongIntNumbersRandom, NUMBER_OF_TESTS)
{
  // get 2 numbers
  auto x = IntRandomNumber<unsigned long int>();
  auto y = IntRandomNumber<long int>();

  auto anyx = ::myodd::dynamic::Any(x);
  auto anyy = ::myodd::dynamic::Any(y);

  if (y > 0 && x > (unsigned long int)y)
  {
    ASSERT_TRUE(anyx > anyy);
    ASSERT_TRUE(anyx > y);
    ASSERT_TRUE(x > anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_TRUE(anyy < anyx);
    ASSERT_TRUE(anyy < x);
    ASSERT_TRUE(y < anyx);
    ASSERT_TRUE(anyy <= anyx);

    ASSERT_FALSE(anyx <= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx < anyy);
    ASSERT_FALSE(anyx < y);
  }

  if (y < 0 || x < (unsigned long int)y)
  {
    ASSERT_TRUE(anyx < anyy);
    ASSERT_TRUE(anyx < y);
    ASSERT_TRUE(x < anyy);
    ASSERT_TRUE(anyx <= anyy);

    ASSERT_FALSE(anyx >= anyy);
    ASSERT_FALSE(anyx == anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(x > anyy);
  }

  if (x == y)
  {
    ASSERT_TRUE(anyx == anyy);
    ASSERT_TRUE(anyx <= anyy);
    ASSERT_TRUE(anyx >= anyy);

    ASSERT_FALSE(anyx < y);
    ASSERT_FALSE(x < anyy);
    ASSERT_FALSE(anyx > anyy);
    ASSERT_FALSE(anyx > y);
  }
}