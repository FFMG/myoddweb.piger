#include "string/string.h"
#include "../testcommon.h"
#include <gtest/gtest.h>

const struct test_compare
{
  std::wstring lhs;
  std::wstring rhs;
  int32_t expected;

  friend std::ostream& operator <<(std::ostream& os, const test_compare& obj)
  {
    return os
      << "lhr : '" << myodd::strings::WString2String(obj.lhs)
      << " rhs : " << myodd::strings::WString2String(obj.rhs)
      << " exp : " << obj.expected;
  }
};

struct MyOddStringSensitiveCompare : testing::Test, testing::WithParamInterface<test_compare>
{
};

struct MyOddStringInsensitiveCompare : testing::Test, testing::WithParamInterface<test_compare>
{
};

TEST_P(MyOddStringSensitiveCompare, CaseSensitiveCompare)
{
  auto lhs = GetParam().lhs;
  auto rhs = GetParam().rhs;
  auto expected = GetParam().expected;

  if (expected < 0)
  {
    ASSERT_LT(myodd::strings::Compare(lhs, rhs, true), 0 );
  }
  else
  if (expected > 0)
  {
    ASSERT_GT(myodd::strings::Compare(lhs, rhs, true), 0);
  }
  else
  {
    ASSERT_EQ(0, myodd::strings::Compare(lhs, rhs, true));
	ASSERT_EQ(0, myodd::strings::Compare(lhs, rhs, false));	//	if case sensitive then it must be zero here as well.
  }
}

TEST_P(MyOddStringInsensitiveCompare, CaseInsensitiveCompare)
{
  auto lhs = GetParam().lhs;
  auto rhs = GetParam().rhs;
  auto expected = GetParam().expected;

  if (expected < 0)
  {
    ASSERT_LT( myodd::strings::Compare(lhs, rhs, false), 0);
  }
  else
  if (expected > 0)
  {
    ASSERT_GT( myodd::strings::Compare(lhs, rhs, false), 0);
  }
  else
  {
    ASSERT_EQ(0, myodd::strings::Compare(lhs, rhs, false));
  }
}

INSTANTIATE_TEST_SUITE_P(CaseInsensitiveCompareAllTheSame, MyOddStringInsensitiveCompare,
  testing::Values(
    test_compare{ L"Hello", L"Hello", 0 },
    test_compare{ L"Hello", L"Hello", 0 },
    test_compare{ L"Hello", L"hello", 0 },
    test_compare{ L"", L"", 0 },
    test_compare{ L"0", L"0", 0 },
	  test_compare{ L"01", L"01", 0 },
	  test_compare{ L"q123", L"Q123", 0 },
	  test_compare{ L"qwErtY", L"qwerty", 0 },
	  test_compare{ L"qwerty", L"QweRty", 0 },
    test_compare{ L"costa rica", L"costa rica", 0 },
  	test_compare{ L"MÖRBYLÅNGA", L"MÖRBYLÅNGA", 0 },
    // from gcc
    test_compare{ L"zzz", L"zzz", 0 },
    test_compare{ L"abCD", L"ABCd", 0 }
  ));

INSTANTIATE_TEST_SUITE_P(CaseSensitiveCompareAllTheSame, MyOddStringSensitiveCompare,
  testing::Values(
    test_compare{ L"Hello", L"Hello", 0 },
    test_compare{ L"Hello", L"Hello", 0 },
    test_compare{ L"", L"", 0 },
    test_compare{ L"0", L"0", 0 },
    test_compare{ L"01", L"01", 0 },
    test_compare{ L"costa rica", L"costa rica", 0 },
	// from gcc
    test_compare{ L"zzz", L"zzz", 0 },
	test_compare{ L"MÖRBYLÅNGA", L"MÖRBYLÅNGA", 0 }
));

INSTANTIATE_TEST_SUITE_P(CaseInsensitiveCompareNotTheSame, MyOddStringInsensitiveCompare,
  testing::Values(
    test_compare{ L"Hello", L"Helo", -1 },
    test_compare{ L"0", L"1", -1 },
    test_compare{ L"1", L"0", 1 },
    // from gcc
	test_compare{ L"costa rica", L"costa marbella", 1 }, // r>m
	test_compare{ L"costa marbella", L"costa rica", -1 },// m<r,
    test_compare{ L"aB", L"Ac", -1 },
    test_compare{ L"ABC", L"ABCd", -1 },
    test_compare{ L"acc", L"abc", 1 },
    test_compare{ L"ABCd", L"abc", 1 } // unlike the test below d>null
    ));

INSTANTIATE_TEST_SUITE_P(CaseSensitiveCompareNotTheSame, MyOddStringSensitiveCompare,
  testing::Values(
    test_compare{ L"Hello", L"Helo", -1 },
    test_compare{ L"Hello", L"hello",-1 },
    test_compare{ L"0", L"1", -1 },
    test_compare{ L"1", L"0", 1 },
    test_compare{ L"q123", L"Q123", 1 },
    test_compare{ L"qwErtY", L"qwerty", -1 },
    test_compare{ L"qwerty", L"QweRty", 1 },
    // from gcc
    test_compare{ L"costa rica", L"costa marbella", 1 }, // r>m
    test_compare{ L"costa marbella", L"costa rica", -1 },// m<r,
    test_compare{ L"aB", L"Ac", 1 },
    test_compare{ L"ABC", L"ABCd", -1 },
    test_compare{ L"acc", L"abc", 1 },
    test_compare{ L"ABCd", L"abc", -1 }, // unlike the test above a<A
    test_compare{ L"abCD", L"ABCd", 1 }
	));