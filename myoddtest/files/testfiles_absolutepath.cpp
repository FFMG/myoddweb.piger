#include "string/string.h"
#include "files/files.h"
#include <gtest/gtest.h>
#include <shlobj.h>

const struct test_absolutepath
{
  std::wstring relative;
  std::wstring origin;
  std::wstring expected;
  bool result;
 
  friend std::ostream& operator <<(std::ostream& os, const test_absolutepath& obj)
  {
    return os
      << "Given : '" << myodd::strings::WString2String(obj.relative) << "'"
      << " Exp : '" << myodd::strings::WString2String(obj.origin) << "'";
  }
};

struct MyOddFilesAbsolutePath : testing::Test, testing::WithParamInterface<test_absolutepath>
{
};

TEST_P(MyOddFilesAbsolutePath, TestAbsolutePath)
{
  auto relative = GetParam().relative;
  auto origin = GetParam().origin;
  auto result = GetParam().result;

  MYODD_STRING dest;
  ASSERT_EQ(result, myodd::files::GetAbsolutePath(dest, relative, origin ));
  if (result)
  {
    auto expected = GetParam().expected;
    ASSERT_EQ(expected, dest);
  }
}

/*
ASSERT(GetAbsolutePath(lpdest, _T("")));
ASSERT(_tcsicmp(lpdest, _T("C:\\Documents and Settings\\All Users\\")) == 0);
*/

INSTANTIATE_TEST_CASE_P(TestAbsolutePath, MyOddFilesAbsolutePath,
  testing::Values(
    test_absolutepath{ L"../../somefile.txt", L"c:\\dira\\dirb\\", L"c:\\somefile.txt", true },
    test_absolutepath{ L"C:\\Documents and Settings\\All Users\\.\\Application Data\\..\\..\\", L"", L"C:\\Documents and Settings\\", true },
    test_absolutepath{ L"somepath2\\", L"C:\\somepath1\\", L"C:\\somepath1\\somepath2\\", true },

    // origin does not matter.
    test_absolutepath{ L"c:\\aaa\\bbb\\ccc\\..\\..\\file.txt", L"\\zzz\\yyy", L"c:\\aaa\\file.txt", true },

    //  null origin
    test_absolutepath{ L"C:\\Documents and Settings\\All Users\\Application Data\\..\\Application Data\\", L"", L"C:\\Documents and Settings\\All Users\\Application Data\\", true },
    test_absolutepath{ L"C:\\Documents and Settings\\All Users\\Application Data\\..\\", L"", L"C:\\Documents and Settings\\All Users\\", true },

    // this should not work because of the depth of the path
    test_absolutepath{ L"../../../somefile.txt", L"c:\\dira\\dirb\\", L"", false },

    //  too deep into itself
    test_absolutepath{ L"c:\\aaa\\bbb\\ccc\\..\\..\\..\\..\\..\\file.txt", L"\\zzz\\yyy", L"\\aaa\\file.txt", false }
));
