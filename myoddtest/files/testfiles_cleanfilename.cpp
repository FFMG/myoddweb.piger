#include "string/string.h"
#include "files/files.h"
#include <gtest/gtest.h>

const struct test_cleanfilename
{
  std::wstring given;
  std::wstring expected;

  friend std::ostream& operator <<(std::ostream& os, const test_cleanfilename& obj)
  {
    return os
      << "Given : '" << myodd::strings::WString2String(obj.given) << "'"
      << " Exp : '" << myodd::strings::WString2String(obj.expected) << "'";
  }
};

struct MyOddFilesCleanFileName : testing::Test, testing::WithParamInterface<test_cleanfilename>
{
};

TEST_P(MyOddFilesCleanFileName, TestCleanFilename)
{
  auto given = GetParam().given;
  auto expected = GetParam().expected;

  std::wstring dest;
  myodd::files::CleanFileName(given);
  ASSERT_EQ(expected, given);
}

INSTANTIATE_TEST_CASE_P(TestCleanFilename, MyOddFilesCleanFileName,
  testing::Values(
    // no changes
    test_cleanfilename{ L"file.txt", L"file.txt" },

    // case sensitive
    test_cleanfilename{ L"FiLe.TxT", L"FiLe.TxT" },

    // '_' is itself not a bad character.
    test_cleanfilename{ L"_._", L"_._" },

    // spaces are allowed.
    test_cleanfilename{ L"file with spaces.txt", L"file with spaces.txt" },
    test_cleanfilename{ L"[bad]^*£", L"_bad___£" },
    test_cleanfilename{ L"[BAD]^*£", L"_BAD___£" }, // case sensitive
    test_cleanfilename{ L"[bad]^*£.exe", L"_bad___£.exe" },
    test_cleanfilename{ L"?[]/\\=+<>:;\",*|^", L"________________" }
  ));