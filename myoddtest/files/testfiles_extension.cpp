#include "string/string.h"
#include "files/files.h"
#include <gtest/gtest.h>

const struct test_extension
{
  std::wstring file;
  std::wstring ext;
  bool is;

  friend std::ostream& operator <<(std::ostream& os, const test_extension& obj)
  {
    return os
      << "File : '" << myodd::strings::WString2String(obj.file) << "'"
      << " Ext : " << myodd::strings::WString2String( obj.ext );
  }
};

struct MyOddFilesIsExtension : testing::Test, testing::WithParamInterface<test_extension>
{
};

struct MyOddFilesGetExtension : testing::Test, testing::WithParamInterface<test_extension>
{
};

TEST_P(MyOddFilesIsExtension, TestIsExtension)
{
  auto file = GetParam().file;
  auto ext = GetParam().ext;
  auto is = GetParam().is;
  // default 'from'

  ASSERT_EQ(is , myodd::files::IsExtension(file, ext ));
}

TEST_P(MyOddFilesGetExtension, TestGetExtension)
{
  auto file = GetParam().file;
  auto ext = GetParam().ext;
  // default 'from'

  ASSERT_EQ(ext, myodd::files::get_extension(file));
}

INSTANTIATE_TEST_CASE_P(TestIsExtension, MyOddFilesIsExtension,
  testing::Values(
    test_extension{ L"", L"", false },
    test_extension{ L"", L"txt", false },
    test_extension{ L"", L".txt", false },
    test_extension{ L"somefile.txt", L".", false },
    test_extension{ L"somefile.txt", L"", false },

    test_extension{ L"somefile.txt", L"ftp", false },
    test_extension{ L"somefile.txt", L".ftp", false },

    test_extension{ L"somefile.txt", L".txt2", false }, // similar
    test_extension{ L"somefile.txt", L".tx", false },
    test_extension{ L"somefile.txt", L"txt2", false },
    test_extension{ L"somefile.txt", L"tx", false },
    
    test_extension{ L"somefile.txt", L"txt", true },
    test_extension{ L"somefile.txt", L".txt", true },
    test_extension{ L"somefile.txt", L".tXt", true },
    test_extension{ L"somefile.txt", L".TXT", true },
    test_extension{ L"somefile.TXT", L".txt", true },
    test_extension{ L"somefile.TXT", L".tXt", true },
    test_extension{ L"somefile.TXT", L".TXT", true },
    test_extension{ L"somefile.TxT", L".txt", true },
    test_extension{ L"somefile.TxT", L".tXt", true },
    test_extension{ L"somefile.TxT", L".TXT", true }
  ));

INSTANTIATE_TEST_CASE_P(TestIsExtensionSpecialCases, MyOddFilesIsExtension,
  testing::Values(
    test_extension{ L"somefile.txt1.text2", L".txt1\\.text2", true },  //  second '.' is escaped.
    test_extension{ L"somefile.txt1.text2", L".txt1.text2", true },  //  second '.' is not escaped.

    test_extension{ L"somefile.txt1.text2", L"txt1\\.text2", true },  //  second '.' is escaped.
    test_extension{ L"somefile.txt1.text2", L"txt1.text2", true }  //  second '.' is not escaped.
));

INSTANTIATE_TEST_CASE_P(TestIgnoreDeadCharacters, MyOddFilesIsExtension,
  testing::Values(
    test_extension{ L"somefile.txt", L"    txt     ", true },
    test_extension{ L"    somefile.txt    ", L"    txt    ", true },
    test_extension{ L"    somefile.txt    ", L"txt", true },
    test_extension{ L"    somefile.txt    ", L"   txt    ", true }
));

INSTANTIATE_TEST_CASE_P(TestGetExtension, MyOddFilesGetExtension,
  testing::Values(
    test_extension{ L"", L"", false },
    test_extension{ L"file.txt", L"txt", false },
    test_extension{ L"file.TxT", L"TxT", false },
    test_extension{ L"file.TXT", L"TXT", false },
    test_extension{ L"file.LONG", L"LONG", false },
    test_extension{ L".LONG", L"LONG", false },
    test_extension{ L".a.b.c", L"c", false },
    test_extension{ L"somefile", L"", false }
));
