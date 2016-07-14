#include "string/string.h"
#include "../testcommon.h"
#include <gtest/gtest.h>

const struct test_replace
{
  std::wstring haystack;
  std::wstring needle;
  std::wstring replace;
  std::wstring expected;

  friend std::ostream& operator <<(std::ostream& os, const test_replace& obj)
  {
    return os
      << "Given : '" << myodd::strings::WString2String(obj.haystack) << "'"
      << " Expected : " << myodd::strings::WString2String(obj.expected);
  }
};

struct MyOddStringCaseSensitiveReplace : testing::Test, testing::WithParamInterface<test_replace>
{
};

struct MyOddStringCaseInSensitiveReplace : testing::Test, testing::WithParamInterface<test_replace>
{
};

TEST_P(MyOddStringCaseSensitiveReplace, CaseSensitiveReplace)
{
  const auto haystack = GetParam().haystack;
  auto needle = GetParam().needle;
  auto replace = GetParam().replace;
  auto expected = GetParam().expected;

  ASSERT_EQ(expected, myodd::strings::Replace(haystack, needle, replace));
}

TEST_P(MyOddStringCaseInSensitiveReplace, CaseInSensitiveReplace)
{
  const auto haystack = GetParam().haystack;
  auto needle = GetParam().needle;
  auto replace = GetParam().replace;
  auto expected = GetParam().expected;

  ASSERT_EQ(expected, myodd::strings::Replace(haystack, needle, replace, false));
}

INSTANTIATE_TEST_CASE_P(VariousReplace, MyOddStringCaseSensitiveReplace,
  testing::Values(
    test_replace{ L"HelloHello", L"Hello", L"No", L"NoNo" },
    test_replace{ L"HelloHello", L"Hello", L"", L"" },
    test_replace{ L"Hellohello", L"Hello", L"No", L"Nohello" }, // second hello is lowercase
    test_replace{ L"Hellohello", L"Hello", L"", L"hello" },
    test_replace{ L"ABC", L"B", L"BB", L"ABBC" },
    test_replace{ L"ABC", L"B", L"XXXX", L"AXXXXC" },
    test_replace{ L"AAA", L"A", L"XXXX", L"XXXXXXXXXXXX" },
    test_replace{ L"ABBC", L"BB", L"B", L"ABC" },
    test_replace{ L"AXXXXC", L"XXXX", L"B", L"ABC" },
    test_replace{ L"XXXXXXXXXXXX", L"XXXX", L"A", L"AAA" },
    test_replace{ L"ABABAB", L"A", L"XX", L"XXBXXBXXB" },
    test_replace{ L"ABC", L"ABC", L"ABC", L"ABC" }, // test recursion
    test_replace{ L"ABC", L"A", L"A", L"ABC" },
    test_replace{ L"ABC", L"B", L"B", L"ABC" },
    test_replace{ L"ABC", L"D", L"X", L"ABC" }    // needle does not exit.
  ));

INSTANTIATE_TEST_CASE_P(VariousReplace, MyOddStringCaseInSensitiveReplace,
  testing::Values(
    test_replace{ L"HelloHello", L"Hello", L"No", L"NoNo" },
    test_replace{ L"HelloHello", L"Hello", L"", L"" },
    test_replace{ L"Hellohello", L"Hello", L"No", L"NoNo" }, // second hello is lowercase
    test_replace{ L"Hellohello", L"Hello", L"", L"" },
    test_replace{ L"ABC", L"B", L"b", L"AbC" },
    test_replace{ L"xAxCX", L"x", L"z", L"zAzCz" },
    test_replace{ L"ABC", L"A", L"A", L"ABC" },
    test_replace{ L"ABC", L"a", L"A", L"ABC" }
));
