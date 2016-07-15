#include "string/string.h"
#include "files/files.h"
#include <gtest/gtest.h>

const struct test_isextension
{
  std::wstring file;
  std::wstring ext;
  bool is;

  friend std::ostream& operator <<(std::ostream& os, const test_isextension& obj)
  {
    return os
      << "File : '" << myodd::strings::WString2String(obj.file) << "'"
      << " Ext : " << myodd::strings::WString2String( obj.ext );
  }
};

struct MyOddFilesIsExtension : testing::Test, testing::WithParamInterface<test_isextension>
{
};

TEST_P(MyOddFilesIsExtension, TestIsExtension)
{
  auto file = GetParam().file;
  auto ext = GetParam().ext;
  auto is = GetParam().is;
  // default 'from'

  ASSERT_EQ(is , myodd::files::is_extension(file, ext ));
}

INSTANTIATE_TEST_CASE_P(TestIsExtension, MyOddFilesIsExtension,
  testing::Values(
    test_isextension{ L"", L"", false },
    test_isextension{ L"", L"txt", false },
    test_isextension{ L"", L".txt", false },
    test_isextension{ L"somefile.txt", L".", false },
    test_isextension{ L"somefile.txt", L"", false },

    test_isextension{ L"somefile.txt", L"ftp", false },
    test_isextension{ L"somefile.txt", L".ftp", false },

    test_isextension{ L"somefile.txt", L".txt2", false }, // similar
    test_isextension{ L"somefile.txt", L".tx", false },
    test_isextension{ L"somefile.txt", L"txt2", false },
    test_isextension{ L"somefile.txt", L"tx", false },
    
    test_isextension{ L"somefile.txt", L"txt", true },
    test_isextension{ L"somefile.txt", L".txt", true },
    test_isextension{ L"somefile.txt", L".tXt", true },
    test_isextension{ L"somefile.txt", L".TXT", true },
    test_isextension{ L"somefile.TXT", L".txt", true },
    test_isextension{ L"somefile.TXT", L".tXt", true },
    test_isextension{ L"somefile.TXT", L".TXT", true },
    test_isextension{ L"somefile.TxT", L".txt", true },
    test_isextension{ L"somefile.TxT", L".tXt", true },
    test_isextension{ L"somefile.TxT", L".TXT", true }
  ));

INSTANTIATE_TEST_CASE_P(TestIsExtensionSpecialCases, MyOddFilesIsExtension,
  testing::Values(
    test_isextension{ L"somefile.txt1.text2", L".txt1\\.text2", true },  //  second '.' is escaped.
    test_isextension{ L"somefile.txt1.text2", L".txt1.text2", true },  //  second '.' is not escaped.

    test_isextension{ L"somefile.txt1.text2", L"txt1\\.text2", true },  //  second '.' is escaped.
    test_isextension{ L"somefile.txt1.text2", L"txt1.text2", true }  //  second '.' is not escaped.
));

INSTANTIATE_TEST_CASE_P(TestIgnoreDeadCharacters, MyOddFilesIsExtension,
  testing::Values(
    test_isextension{ L"somefile.txt", L"    txt     ", true },
    test_isextension{ L"    somefile.txt    ", L"    txt    ", true },
    test_isextension{ L"    somefile.txt    ", L"txt", true },
    test_isextension{ L"    somefile.txt    ", L"   txt    ", true }
));
