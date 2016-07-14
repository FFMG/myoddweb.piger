#include "string\string.h"
#include "..\testcommon.h"
#include <gtest/gtest.h>

const struct test_explode
{
  std::wstring actual;
  wchar_t delim;
  std::vector<std::wstring> expected;
  size_t len;
};

struct MyOddStringExplode : testing::Test, testing::WithParamInterface<test_explode>
{
  MyOddStringExplode()
  {
  }

  ~MyOddStringExplode()
  {
  }
};

TEST_P(MyOddStringExplode, Default)
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

INSTANTIATE_TEST_CASE_P(Default, MyOddStringExplode,
  testing::Values(
    test_explode{ L",, ,A,", L',', {L"",L"",L" ",L"A",L""}, 5 }
  ));
