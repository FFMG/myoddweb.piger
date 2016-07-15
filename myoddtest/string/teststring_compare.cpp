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

struct MyOddStringInsensitiveCompare : testing::Test, testing::WithParamInterface<test_compare>
{
};

TEST_P(MyOddStringInsensitiveCompare, CaseInsensitiveCompare)
{
  auto lhs = GetParam().lhs;
  auto rhs = GetParam().rhs;
  auto expected = GetParam().expected;

  ASSERT_EQ(expected, myodd::strings::Compare(lhs, rhs, false ));
}

INSTANTIATE_TEST_CASE_P(CaseInsensitiveCompareAllTheSame, MyOddStringInsensitiveCompare,
  testing::Values(
    test_compare{ L"Hello", L"Hello", 0 },
    test_compare{ L"Hello", L"hello", 0 },
    test_compare{ L"", L"", 0 },
    test_compare{ L"0", L"0", 0 },
    test_compare{ L"costa rica", L"costa rica", 0 },
    test_compare{ L"costa rica", L"costa marbella", 5 }, // r>m
    test_compare{ L"costa marbella", L"costa rica", -5 } // m<r
  ));

INSTANTIATE_TEST_CASE_P(CaseInsensitiveCompareNotTheSame, MyOddStringInsensitiveCompare,
  testing::Values(
    test_compare{ L"Hello", L"Hello", 0 },
    test_compare{ L"Hello", L"Helo", -3 },
    test_compare{ L"0", L"1", -1 },
    test_compare{ L"1", L"0", 1 }
    ));