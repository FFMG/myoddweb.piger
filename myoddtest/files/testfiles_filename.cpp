#include "string/string.h"
#include "files/files.h"
#include <gtest/gtest.h>
#include <shlobj.h>

const struct test_filename
{
  std::wstring given;
  std::wstring expected;

  bool expand;

  friend std::ostream& operator <<(std::ostream& os, const test_filename& obj)
  {
    return os
      << "Given : '" << myodd::strings::WString2String(obj.given) << "'"
      << " Exp : '" << myodd::strings::WString2String(obj.expected) << "'";
  }
};

struct MyOddFilesFileName : testing::Test, testing::WithParamInterface<test_filename>
{
};

struct MyOddFilesFileNameNotSame : testing::Test, testing::WithParamInterface<test_filename>
{
};

TEST_P(MyOddFilesFileName, TestFileName)
{
  auto given = GetParam().given;
  auto expected = GetParam().expected;
  auto expand = GetParam().expand;
  
  ASSERT_EQ(expected, myodd::files::GetFileName(given, expand ));
}

TEST_P(MyOddFilesFileNameNotSame, TestNotSameFileName)
{
  auto given = GetParam().given;
  auto expected = GetParam().expected;
  auto expand = GetParam().expand;

  ASSERT_NE(expected, myodd::files::GetFileName(given, expand));
}

// mainly to check that we don't alway return true.
INSTANTIATE_TEST_SUITE_P(TestFileName, MyOddFilesFileNameNotSame,
  testing::Values(
    test_filename{ L"\\somthing\\filename.TxT", L"filename.txt", false },
    test_filename{ L"c:\\a\\b\\something.txt", L"\\something.txt", false },
    test_filename{ L"c:\\a\\b\\Something.TxT", L"something.txt", false },
    test_filename{ L"%COMSPEC%", L"cmd.exe", false } //  not expended
));

INSTANTIATE_TEST_SUITE_P(TestFileName, MyOddFilesFileName,
  testing::Values(
    test_filename{ L"\\somthing\\.txt", L".txt", false },
    test_filename{ L"\\somthing\\filename.txt", L"filename.txt", false },
    test_filename{ L"\\somthing\\filename.TxT", L"filename.TxT", false },
    test_filename{ L"\\.txt", L".txt", false },
    test_filename{ L"c:\\path\\only\\", L"", false },
    test_filename{ L"\\something.txt", L"something.txt", false },
    test_filename{ L"c:\\a\\b\\something.txt", L"something.txt", false },
    test_filename{ L"c:\\a\\b\\Something.TxT", L"Something.TxT", false },
    test_filename{ L"%COMSPEC%", L"cmd.exe", true }, // expended.
    test_filename{ L"%COMSPEC%", L"", false} //  not expended
));
