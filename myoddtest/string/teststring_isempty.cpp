#include "string/string.h"
#include "../testcommon.h"
#include <gtest/gtest.h>

const struct test_is_empty_wchar_t
{
  const wchar_t* given;
  bool is;

  friend std::ostream& operator <<(std::ostream& os, const test_is_empty_wchar_t& obj)
  {
    return os
      << "Given : '" << (obj.given ? myodd::strings::WString2String(obj.given) : "(null)") << "'"
      << " Expected : " << (obj.is ? "true" : "false");
  }
};

const struct test_is_empty_wstring
{
  std::wstring given;
  bool is;

  friend std::ostream& operator <<(std::ostream& os, const test_is_empty_wstring& obj)
  {
    return os
      << "Given : '" << myodd::strings::WString2String(obj.given) << "'"
      << " Expected : " << (obj.is ? "true" : "false");
  }
};

struct MyOddStringWCharIsEmpty : testing::Test, testing::WithParamInterface<test_is_empty_wchar_t>
{
};

struct MyOddStringWideStringIsEmpty : testing::Test, testing::WithParamInterface<test_is_empty_wstring>
{
};

TEST_P(MyOddStringWCharIsEmpty, IsEmptyDefaultParams)
{
  auto given = GetParam().given;
  auto is = GetParam().is;

  ASSERT_EQ(is, myodd::strings::IsEmptyString(given));
}

TEST_P(MyOddStringWideStringIsEmpty, IsEmptyDefaultParams)
{
  auto given = GetParam().given;
  auto is = GetParam().is;

  ASSERT_EQ(is, myodd::strings::IsEmptyString(given));
}

INSTANTIATE_TEST_CASE_P(CheckEmptyWChar, MyOddStringWCharIsEmpty,
  testing::Values(
    test_is_empty_wchar_t{ L"     ", true },
    test_is_empty_wchar_t{ L"", true },
    test_is_empty_wchar_t{ L"        A", false },
    test_is_empty_wchar_t{ L"A", false },
    test_is_empty_wchar_t{ L"A        ", false },
    test_is_empty_wchar_t{ L"        A         ", false },
    test_is_empty_wchar_t{ L"                  ", true },
    test_is_empty_wchar_t{ nullptr, true }
));

INSTANTIATE_TEST_CASE_P(CheckEmptyWStrings, MyOddStringWideStringIsEmpty,
  testing::Values(
    test_is_empty_wstring{ L"     ", true },
    test_is_empty_wstring{ L"", true },
    test_is_empty_wstring{ L"        A", false },
    test_is_empty_wstring{ L"A", false },
    test_is_empty_wstring{ L"A        ", false },
    test_is_empty_wstring{ L"        A         ", false },
    test_is_empty_wstring{ L"                  ", true }
));
