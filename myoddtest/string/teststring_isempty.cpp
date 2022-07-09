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

const struct test_is_empty_char
{
  const char* given;
  bool is;

  friend std::ostream& operator <<(std::ostream& os, const test_is_empty_char& obj)
  {
    return os
      << "Given : '" << (obj.given ? obj.given : "(null)") << "'"
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

const struct test_is_empty_string
{
  std::string given;
  bool is;

  friend std::ostream& operator <<(std::ostream& os, const test_is_empty_string& obj)
  {
    return os
      << "Given : '" << obj.given << "'"
      << " Expected : " << (obj.is ? "true" : "false");
  }
};

struct MyOddStringWideCharIsEmpty : testing::Test, testing::WithParamInterface<test_is_empty_wchar_t>
{
};

struct MyOddStringWideStringIsEmpty : testing::Test, testing::WithParamInterface<test_is_empty_wstring>
{
};

struct MyOddStringCharIsEmpty : testing::Test, testing::WithParamInterface<test_is_empty_char>
{
};

struct MyOddStringStringIsEmpty : testing::Test, testing::WithParamInterface<test_is_empty_string>
{
};

TEST_P(MyOddStringCharIsEmpty, TestStringCharIsEmpty)
{
  auto given = GetParam().given;
  auto is = GetParam().is;

  ASSERT_EQ(is, myodd::strings::IsEmptyString(given));
}

TEST_P(MyOddStringWideCharIsEmpty, IsEmptyDefaultParams)
{
  auto given = GetParam().given;
  auto is = GetParam().is;

  ASSERT_EQ(is, myodd::strings::IsEmptyString(given));
}

TEST_P(MyOddStringStringIsEmpty, IsEmptyDefaultParams)
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

INSTANTIATE_TEST_SUITE_P(CheckEmptyWChar, MyOddStringWideCharIsEmpty,
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

INSTANTIATE_TEST_SUITE_P(CheckEmptyChar, MyOddStringCharIsEmpty,
  testing::Values(
    test_is_empty_char{ "     ", true },
    test_is_empty_char{ "", true },
    test_is_empty_char{ "        A", false },
    test_is_empty_char{ "A", false },
    test_is_empty_char{ "A        ", false },
    test_is_empty_char{ "        A         ", false },
    test_is_empty_char{ "                  ", true },
    test_is_empty_char{ nullptr, true }
));

INSTANTIATE_TEST_SUITE_P(CheckEmptyWideStrings, MyOddStringWideStringIsEmpty,
  testing::Values(
    test_is_empty_wstring{ L"     ", true },
    test_is_empty_wstring{ L"", true },
    test_is_empty_wstring{ L"        A", false },
    test_is_empty_wstring{ L"A", false },
    test_is_empty_wstring{ L"A        ", false },
    test_is_empty_wstring{ L"        A         ", false },
    test_is_empty_wstring{ L"                  ", true }
));

INSTANTIATE_TEST_SUITE_P(CheckEmptyStrings, MyOddStringStringIsEmpty,
  testing::Values(
    test_is_empty_string{ "     ", true },
    test_is_empty_string{ "", true },
    test_is_empty_string{ "        A", false },
    test_is_empty_string{ "A", false },
    test_is_empty_string{ "A        ", false },
    test_is_empty_string{ "        A         ", false },
    test_is_empty_string{ "                  ", true }
));
