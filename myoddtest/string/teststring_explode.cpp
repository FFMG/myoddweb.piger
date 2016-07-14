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


TEST_P(MyOddStringExplode, DefaultParams)
{
  auto param = GetParam();

  auto actual = GetParam().actual;
  auto delim = GetParam().delim;
  auto expected = GetParam().expected;
  auto len = GetParam().len;

  std::vector<std::wstring> s;
  auto l = myodd::strings::explode(s, actual, delim);

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
  auto l = myodd::strings::explode(s, actual, delim, count);

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
  auto l = myodd::strings::explode(s, actual, delim, count, addEmpty);

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

INSTANTIATE_TEST_CASE_P(MakeSureThatMinu1DoesNotChangeAnything, MyOddStringExplodeWithCount,
  testing::Values(
    test_explode{ L",, ,A,", L',',{ L"",L"",L" ",L"A",L"" }, 5, -1 },
    test_explode{ L",,,A,", L',',{ L"",L"",L"",L"A",L"" }, 5, -1 },
    test_explode{ L",", L',',{ L"",L"" }, 2, -1 },
    test_explode{ L"1,2", L',',{ L"1",L"2" }, 2, -1 },
    test_explode{ L"", L',',{ L"" }, 1, -1 },
    test_explode{ L",, ,A,", L',',{ L"",L"",L" ",L"A",L"" }, 5, -1 }
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

INSTANTIATE_TEST_CASE_P(VariousCountSize, MyOddStringExplodeWithAddEmpty,
  testing::Values(
    test_explode{ L"1,2,3,4,5", L',',{ L"1",L"2",L"3",L"4",L"5" }, 5, -1, false }, //  no empties
    test_explode{ L"1,,,4,5", L',',{ L"1",L"4",L"5" }, 3, -1, false },
    test_explode{ L",,,,", L',',{ }, 0, -1, false },
    test_explode{ L",,,,", L',',{L"",L"",L"",L"",L"" }, 5, -1, true },  //  we want empties
    test_explode{ L",, ,,", L',',{ L" " }, 1, -1, false },  //  a space is not empty
    test_explode{ L"", L',',{}, 0, -1, false }
));
