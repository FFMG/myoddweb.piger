#include "string/string.h"
#include "files/files.h"
#include <gtest/gtest.h>
#include <shlobj.h>

const struct test_basefromfile
{
  std::wstring given;
  std::wstring expected;

  bool expand;
  bool trailing;

  friend std::ostream& operator <<(std::ostream& os, const test_basefromfile& obj)
  {
    return os
      << "Given : '" << myodd::strings::WString2String(obj.given) << "'"
      << " Exp : '" << myodd::strings::WString2String(obj.expected) << "'";
  }
};

struct MyOddFilesBaseFromFile : testing::Test, testing::WithParamInterface<test_basefromfile>
{
};

struct MyOddFilesBaseFromFileExpandAppData : testing::Test, testing::WithParamInterface<test_basefromfile>
{
};

TEST_P(MyOddFilesBaseFromFile, TestBaseFromFile)
{
  auto given = GetParam().given;
  auto expected = GetParam().expected;
  auto expand = GetParam().expand;
  auto trailing = GetParam().trailing;

  ASSERT_EQ(expected, myodd::files::GetBaseFromFile(given, expand, trailing ));
}

TEST_P(MyOddFilesBaseFromFileExpandAppData, TestBaseFromFileAndExpand)
{
  auto given = GetParam().given;
  auto expected = GetParam().expected;
  auto expand = GetParam().expand;
  if( expand )
  {
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
    {
      expected = myodd::strings::Replace(expected, L"%appdata%", szPath);
    }
  }
  auto trailing = GetParam().trailing;

  ASSERT_EQ(expected, myodd::files::GetBaseFromFile(given, expand, trailing));
}

INSTANTIATE_TEST_CASE_P(TestBaseFromFile, MyOddFilesBaseFromFileExpandAppData,
  testing::Values(
    test_basefromfile{ L"%appdata%\\filename.txt", L"%appdata%\\", true, true },
    test_basefromfile{ L"%appdata%\\filename.txt", L"%appdata%\\", false, true },
    test_basefromfile{ L"%appdata%\\filename.txt", L"%appdata%", true, false },
    test_basefromfile{ L"%appdata%\\filename.txt", L"%appdata%", false, false }
));

INSTANTIATE_TEST_CASE_P(TestBaseFromFile, MyOddFilesBaseFromFile,
  testing::Values(
    test_basefromfile{ L"filename.txt", L"\\", false, true },
    test_basefromfile{ L"C:\\", L"C:\\", false, true },
    test_basefromfile{ L"C:\\", L"C:", false, false },
    test_basefromfile{ L"C:\\dir/test.txt", L"C:\\dir\\", false, true },
    test_basefromfile{ L"c:\\a\\b\\something.txt", L"c:\\a\\b\\", false, true },
    test_basefromfile{ L"c:\\a\\b\\something.txt", L"c:\\a\\b", false, false },
    test_basefromfile{ L"c:\\a\\b\\something.txt\\", L"c:\\a\\b\\something.txt\\", false, true },
    test_basefromfile{ L"c:\\a\\b\\something.txt\\", L"c:\\a\\b\\something.txt", false, false },
    test_basefromfile{ L"C:\\test.txt", L"C:\\", true, true }
  ));
