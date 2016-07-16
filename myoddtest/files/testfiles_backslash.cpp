#include "string/string.h"
#include "files/files.h"
#include <gtest/gtest.h>

const struct test_backslash
{
  std::wstring given;
  std::wstring expected;

  friend std::ostream& operator <<(std::ostream& os, const test_backslash& obj)
  {
    return os
      << "Given : '" << myodd::strings::WString2String(obj.given) << "'"
      << " Expected : " << myodd::strings::WString2String( obj.expected );
  }
};

struct MyOddFilesRemoveLeadingBackslash : testing::Test, testing::WithParamInterface<test_backslash>
{
};

struct MyOddFilesRemoveTrailingBackslash : testing::Test, testing::WithParamInterface<test_backslash>
{
};

struct MyOddFilesAddTrailingBackslash : testing::Test, testing::WithParamInterface<test_backslash>
{
};

TEST_P(MyOddFilesAddTrailingBackslash, TestAddTrailingBackSlash)
{
  auto expected = GetParam().expected;
  auto given = GetParam().given;

  myodd::files::AddTrailingBackSlash(given);
  ASSERT_EQ(expected, given);
}

TEST_P(MyOddFilesRemoveLeadingBackslash, TestRemoveLeadingBackSlash)
{
  auto expected = GetParam().expected;
  auto given = GetParam().given;

  myodd::files::RemoveLeadingBackSlash(given);
  ASSERT_EQ(expected, given);
}

TEST_P(MyOddFilesRemoveTrailingBackslash, TestRemoveTrailingBackSlash)
{
  auto expected = GetParam().expected;
  auto given = GetParam().given;

  myodd::files::RemoveTrailingBackSlash(given);
  ASSERT_EQ(expected, given);
}

INSTANTIATE_TEST_CASE_P(TestRemoveLeadingBackSlash, MyOddFilesRemoveLeadingBackslash,
  testing::Values(
    test_backslash{ L"", L"" },
    test_backslash{ L"\\", L"" },
    test_backslash{ L"\\\\", L"" },
    test_backslash{ L"\\Hello", L"Hello" },
    test_backslash{ L"\\HeLlO", L"HeLlO" },
    test_backslash{ L"\\Hello\\", L"Hello\\" },
    test_backslash{ L"\\\\Hello", L"Hello" },

    test_backslash{ L"/", L"" },
    test_backslash{ L"//", L"" },
    test_backslash{ L"/Hello", L"Hello" },
    test_backslash{ L"/HeLlO", L"HeLlO" },
    test_backslash{ L"/Hello/", L"Hello/" },
    test_backslash{ L"//Hello", L"Hello" },

    test_backslash{ L"\\/Hello", L"Hello" },
    test_backslash{ L"/\\Hello", L"Hello" }
  ));

INSTANTIATE_TEST_CASE_P(TestRemoveTrailingBackSlash, MyOddFilesRemoveTrailingBackslash,
  testing::Values(
    test_backslash{ L"", L"" },
    test_backslash{ L"\\", L"" },
    test_backslash{ L"\\\\", L"" },
    test_backslash{ L"Hello\\", L"Hello" },
    test_backslash{ L"HeLlO\\", L"HeLlO" },
    test_backslash{ L"\\Hello\\", L"\\Hello" },
    test_backslash{ L"Hello\\\\", L"Hello" },

    test_backslash{ L"/", L"" },
    test_backslash{ L"//", L"" },
    test_backslash{ L"Hello/", L"Hello" },
    test_backslash{ L"HeLlO/", L"HeLlO" },
    test_backslash{ L"/Hello/", L"/Hello" },
    test_backslash{ L"Hello//", L"Hello" },

    test_backslash{ L"Hello\\/", L"Hello" },
    test_backslash{ L"Hello/\\", L"Hello" }

));

INSTANTIATE_TEST_CASE_P(TestAddTrailingBackSlash, MyOddFilesAddTrailingBackslash,
  testing::Values(
    test_backslash{ L"", L"\\" },
    test_backslash{ L"Hello", L"Hello\\" },
    test_backslash{ L"Hello\\", L"Hello\\" },
    test_backslash{ L"Hello\\\\", L"Hello\\" },

    test_backslash{ L"Hello\\/\\", L"Hello\\" },
    test_backslash{ L"Hello/", L"Hello\\" },
    test_backslash{ L"Hello//", L"Hello\\" }
));