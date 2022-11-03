#include "string\string.h"
#include "..\testcommon.h"
#include <gtest/gtest.h>

const struct test_explode
{
  std::wstring actual;
  wchar_t delim;
  std::vector<std::wstring> expected;
  size_t len;
  int count;
  bool addEmpty;
};

struct MyOddStringExplodeTest : testing::Test, testing::WithParamInterface<test_explode>
{
};

struct MyOddStringExplodeWithCount : MyOddStringExplodeTest
{
};

struct MyOddStringExplodeWithAddEmpty : MyOddStringExplodeTest
{
};

struct MyOddStringExplodeWithNegativeCount: testing::Test, testing::WithParamInterface<test_explode>
{
};

TEST_P(MyOddStringExplodeWithNegativeCount, TestExplodeWithNegativeCount)
{
  auto param = GetParam();

  auto actual = GetParam().actual;
  auto delim = GetParam().delim;
  auto expected = GetParam().expected;
  auto len = GetParam().len;
  auto count = GetParam().count;

  std::vector<std::wstring> s;
  auto l = myodd::strings::Explode(s, actual, delim, count);

  ASSERT_EQ(expected, s);
  ASSERT_EQ(len, l);
}

TEST_P(MyOddStringExplodeTest, ExplodeStringDefaultParams)
{
  auto param = GetParam();

  auto actual = GetParam().actual;
  auto delim = GetParam().delim;
  auto expected = GetParam().expected;
  auto len = GetParam().len;

  std::vector<std::wstring> s;
  auto l = myodd::strings::Explode(s, actual, delim);

  ASSERT_EQ(expected, s);
  ASSERT_EQ(len, l);
}

TEST_P(MyOddStringExplodeWithCount, CountParams)
{
  auto param = GetParam();

  auto actual = GetParam().actual;
  auto delim = GetParam().delim;
  auto expected = GetParam().expected;
  auto len = GetParam().len;
  auto count = GetParam().count;

  std::vector<std::wstring> s;
  auto l = myodd::strings::Explode(s, actual, delim, count);

  ASSERT_EQ(expected, s);
  ASSERT_EQ(len, l);
}

TEST_P(MyOddStringExplodeWithAddEmpty, CountParams)
{
  auto param = GetParam();

  auto actual = GetParam().actual;
  auto delim = GetParam().delim;
  auto expected = GetParam().expected;
  auto len = GetParam().len;
  auto count = GetParam().count;
  auto addEmpty = GetParam().addEmpty;

  std::vector<std::wstring> s;
  auto l = myodd::strings::Explode(s, actual, delim, count, addEmpty);

  ASSERT_EQ(expected, s);
  ASSERT_EQ(len, l);
}

INSTANTIATE_TEST_SUITE_P(ExplodeStringDefault, MyOddStringExplodeTest,
  testing::Values(
    test_explode{ L",, ,A,", L',', {L"",L"",L" ",L"A",L""}, 5 },
    test_explode{ L",,,A,", L',',{ L"",L"",L"",L"A",L"" }, 5 },
    test_explode{ L",", L',',{ L"",L"" }, 2 },
    test_explode{ L"1,2", L',',{ L"1",L"2" }, 2 },
    test_explode{ L"", L',',{ L"" }, 1 },
    test_explode{ L",, ,A,", L',',{ L"",L"",L" ",L"A",L"" }, 5 }
  ));

INSTANTIATE_TEST_SUITE_P(MakeSureThatMaxIntDoesNotChangeAnything, MyOddStringExplodeWithCount,
  testing::Values(
    test_explode{ L",, ,A,", L',',{ L"",L"",L" ",L"A",L"" }, 5, MYODD_MAX_INT32 },
    test_explode{ L",,,A,", L',',{ L"",L"",L"",L"A",L"" }, 5, MYODD_MAX_INT32 },
    test_explode{ L",", L',',{ L"",L"" }, 2, MYODD_MAX_INT32 },
    test_explode{ L"1,2", L',',{ L"1",L"2" }, 2, MYODD_MAX_INT32 },
    test_explode{ L"", L',',{ L"" }, 1, MYODD_MAX_INT32 },
    test_explode{ L",, ,A,", L',',{ L"",L"",L" ",L"A",L"" }, 5, MYODD_MAX_INT32 }
));

INSTANTIATE_TEST_SUITE_P(VariousCountSizeWithCount, MyOddStringExplodeWithCount,
  testing::Values(
    test_explode{ L",, ,A,", L',',{ L"",L", ,A," }, 2, 2 }
));

INSTANTIATE_TEST_SUITE_P(VariousCountSizeWithNegativeCount, MyOddStringExplodeWithNegativeCount,
  testing::Values(
    test_explode{ L"Abcd1,Abcd2,Abcd3", L',',{ L"Abcd1,Abcd2,Abcd3" }, 1, 0 }, // longer than one char
    test_explode{ L"1,2,3,4,5", L',',{ L"1", L"2", L"3" }, 3, -2 },
    test_explode{ L"1,2,3,4,5", L',',{ }, 0, -200 }, // more than we can actually get
    test_explode{ L"1,2,3,4,5", L',',{ L"1" }, 1, -4 },
    test_explode{ L"1,2,3,4,5", L',',{ }, 0, -5 },
    test_explode{ L"1,2,3,4,5", L',',{ L"1,2,3,4,5" }, 1, 0 }
));

INSTANTIATE_TEST_SUITE_P(VariousCountSizeWithAddEmptyFlag, MyOddStringExplodeWithAddEmpty,
  testing::Values(
    test_explode{ L"1,2,3,4,5", L',',{ L"1",L"2",L"3",L"4",L"5" }, 5, MYODD_MAX_INT32, false }, //  no empties
    test_explode{ L"1,,,4,5", L',',{ L"1",L"4",L"5" }, 3, MYODD_MAX_INT32, false },
    test_explode{ L",,,,", L',',{ }, 0, MYODD_MAX_INT32, false },
    test_explode{ L",,,,", L',',{L"",L"",L"",L"",L"" }, 5, MYODD_MAX_INT32, true },  //  we want empties
    test_explode{ L",, ,,", L',',{ L" " }, 1, MYODD_MAX_INT32, false },  //  a space is not empty
    test_explode{ L"", L',',{}, 0, MYODD_MAX_INT32, false }
));

TEST(ExplodeString, NegativeCountWillReturnTheTotalNumberLessTheCount)
{
  std::vector<std::wstring> s;
  const auto l = myodd::strings::Explode(
    s,
    L"1,2,3,4,5", L',', -2);

  // we have 5 items in the array, we want -2 numbers
  // in other words, we want 5 - 2 = 3
  std::vector<std::wstring> expected = { L"1",L"2",L"3" };

  ASSERT_EQ(3, l);
  ASSERT_EQ(expected, s);
};

TEST(ExplodeString, NegativeCountBiggerThanTheToalWillReturnNothing)
{
  std::vector<std::wstring> s;
  const auto l = myodd::strings::Explode(
    s,
    L"1,2,3,4,5", L',', -10);

  // we have 5 items in the array, we want -10 numbers
  // in other words, we want 5 - 10 = -5
  // we can't do that, so we will return noting
  std::vector<std::wstring> expected = {};

  ASSERT_EQ(0, l);
  ASSERT_EQ(expected, s);
};

TEST(ExplodeString, ZeroCountWillReturnSingleString)
{
  std::vector<std::wstring> s;
  const auto l = myodd::strings::Explode(
    s,
    L"1,2,3,4,5", L',', 0);

  std::vector<std::wstring> expected = { L"1,2,3,4,5" };

  ASSERT_EQ(1, l);
  ASSERT_EQ(expected, s);
};
