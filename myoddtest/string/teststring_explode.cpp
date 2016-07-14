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

struct MyOddStringExplode : testing::Test, testing::WithParamInterface<test_explode>
{
};

struct MyOddStringExplodeWithCount : MyOddStringExplode
{
};

struct MyOddStringExplodeWithAddEmpty : MyOddStringExplode
{
};

struct MyOddStringExplodeWithNegativeCount: MyOddStringExplode
{
};

TEST_P(MyOddStringExplode, DefaultParams)
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

INSTANTIATE_TEST_CASE_P(Default, MyOddStringExplode,
  testing::Values(
    test_explode{ L",, ,A,", L',', {L"",L"",L" ",L"A",L""}, 5 },
    test_explode{ L",,,A,", L',',{ L"",L"",L"",L"A",L"" }, 5 },
    test_explode{ L",", L',',{ L"",L"" }, 2 },
    test_explode{ L"1,2", L',',{ L"1",L"2" }, 2 },
    test_explode{ L"", L',',{ L"" }, 1 },
    test_explode{ L",, ,A,", L',',{ L"",L"",L" ",L"A",L"" }, 5 }
  ));

INSTANTIATE_TEST_CASE_P(MakeSureThatMaxIntDoesNotChangeAnything, MyOddStringExplodeWithCount,
  testing::Values(
    test_explode{ L",, ,A,", L',',{ L"",L"",L" ",L"A",L"" }, 5, MYODD_MAX_INT32 },
    test_explode{ L",,,A,", L',',{ L"",L"",L"",L"A",L"" }, 5, MYODD_MAX_INT32 },
    test_explode{ L",", L',',{ L"",L"" }, 2, MYODD_MAX_INT32 },
    test_explode{ L"1,2", L',',{ L"1",L"2" }, 2, MYODD_MAX_INT32 },
    test_explode{ L"", L',',{ L"" }, 1, MYODD_MAX_INT32 },
    test_explode{ L",, ,A,", L',',{ L"",L"",L" ",L"A",L"" }, 5, MYODD_MAX_INT32 }
));

INSTANTIATE_TEST_CASE_P(VariousCountSize, MyOddStringExplodeWithCount,
  testing::Values(
    test_explode{ L",, ,A,", L',',{ L"",L", ,A," }, 2, 2 },
    test_explode{ L"1,2,3,4,5", L',',{ L"1",L"2,3,4,5" }, 2, 2 },
    test_explode{ L"1,2,3,4,5", L',',{ L"1",L"2",L"3",L"4",L"5" }, 5, 20 }, // count is greater than actual return
    test_explode{ L"1,2,3,4,5", L',',{ L"1,2,3,4,5" }, 1, 1 },
    test_explode{ L"1,2,3,4,5", L',',{ L"1,2,3,4,5" }, 1, 0 },  //  If the limit parameter is zero, then this is treated as 1. 
    test_explode{ L"", L',',{ L"" }, 1, 0 }
));

INSTANTIATE_TEST_CASE_P(VariousCountSize, MyOddStringExplodeWithNegativeCount,
  testing::Values(
    test_explode{ L"1,2,3,4,5", L',',{ L"1", L"2", L"3" }, 3, -2 },
    test_explode{ L"1,2,3,4,5", L',',{ }, 0, -200 },
    test_explode{ L"1,2,3,4,5", L',',{ L"1" }, 1, -4 },
    test_explode{ L"1,2,3,4,5", L',',{ }, 0, -5 },
    test_explode{ L"1,2,3,4,5", L',',{ L"1,2,3,4,5" }, 1, 0 }
));

INSTANTIATE_TEST_CASE_P(VariousCountSize, MyOddStringExplodeWithAddEmpty,
  testing::Values(
    test_explode{ L"1,2,3,4,5", L',',{ L"1",L"2",L"3",L"4",L"5" }, 5, MYODD_MAX_INT32, false }, //  no empties
    test_explode{ L"1,,,4,5", L',',{ L"1",L"4",L"5" }, 3, MYODD_MAX_INT32, false },
    test_explode{ L",,,,", L',',{ }, 0, MYODD_MAX_INT32, false },
    test_explode{ L",,,,", L',',{L"",L"",L"",L"",L"" }, 5, MYODD_MAX_INT32, true },  //  we want empties
    test_explode{ L",, ,,", L',',{ L" " }, 1, MYODD_MAX_INT32, false },  //  a space is not empty
    test_explode{ L"", L',',{}, 0, MYODD_MAX_INT32, false }
));
