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

const struct test_addextension
{
  std::wstring file;
  std::wstring ext;
  bool strip;
  std::wstring expected;

  friend std::ostream& operator <<(std::ostream& os, const test_addextension& obj)
  {
    return os
      << "File : '" << myodd::strings::WString2String(obj.file) << "'"
      << " Ext : " << myodd::strings::WString2String(obj.ext);
  }
};

struct MyOddFilesStripExtension : testing::Test, testing::WithParamInterface<test_extension>
{
};

struct MyOddFilesIsExtension : testing::Test, testing::WithParamInterface<test_extension>
{
};

struct MyOddFilesGetExtension : testing::Test, testing::WithParamInterface<test_extension>
{
};

struct MyOddFilesAddExtension : testing::Test, testing::WithParamInterface<test_addextension>
{
};

TEST_P(MyOddFilesStripExtension, TestStripExtension)
{
  auto file = GetParam().file;
  auto ext = GetParam().ext;

  myodd::files::StripExtension(file);
  ASSERT_EQ(ext, file );
}

TEST_P(MyOddFilesAddExtension, TestAddExtension)
{
  auto file = GetParam().file;
  auto expected = GetParam().expected;
  auto ext = GetParam().ext;
  auto strip = GetParam().strip;

  myodd::files::AddExtension(file, ext, strip );
  ASSERT_EQ(expected, file);
}


TEST_P(MyOddFilesIsExtension, TestIsExtension)
{
  auto file = GetParam().file;
  auto ext = GetParam().ext;
  auto is = GetParam().is;

  ASSERT_EQ(is , myodd::files::IsExtension(file, ext ));
}

TEST_P(MyOddFilesGetExtension, TestGetExtension)
{
  auto file = GetParam().file;
  auto ext = GetParam().ext;

  ASSERT_EQ(ext, myodd::files::get_extension(file));
}

INSTANTIATE_TEST_CASE_P(TestAddExtension, MyOddFilesAddExtension,
  testing::Values(
    test_addextension{ L"filename", L"txt", false, L"filename.txt" }, // no strip
    test_addextension{ L"filename", L"txt", true, L"filename.txt" },  // strip, but nothing to strip 
    test_addextension{ L"filename", L".txt", false, L"filename.txt" },
    test_addextension{ L"filename", L".txt", true, L"filename.txt" },
    test_addextension{ L"filename.", L".txt", true, L"filename.txt" },
    test_addextension{ L"filename.", L".txt", false, L"filename.txt" },
    test_addextension{ L"filename.ext", L"txt", true, L"filename.txt" },
    test_addextension{ L"filename.ext", L".txt", true, L"filename.txt" },
    test_addextension{ L"filename.ext", L"txt", false, L"filename.ext.txt" },
    test_addextension{ L"filename.ext", L".txt", false, L"filename.ext.txt" },
    test_addextension{ L"filename.", L"txt", true, L"filename.txt" },
    test_addextension{ L"filename.", L"txt", false, L"filename.txt" }
  ));

INSTANTIATE_TEST_CASE_P(TestStripExtension, MyOddFilesStripExtension,
  testing::Values(
    test_extension{ L"", L"" },
    test_extension{ L"filename.ext", L"filename" },
    test_extension{ L"FileName.ext", L"FileName" },
    test_extension{ L"filename.ext1.ext2", L"filename.ext1" },
    test_extension{ L".ext", L".ext" },
    test_extension{ L".ExT", L".ExT" },
    test_extension{ L".1", L".1" },
    test_extension{ L"1.1", L"1" },
    test_extension{ L"x.x", L"x" },
    test_extension{ L"12345._", L"12345" },
    test_extension{ L"_._", L"_" },
    test_extension{ L"_.", L"_" },
    test_extension{ L".ext1.ext2", L".ext1" },
    test_extension{ L"z:\\path\\to\\filename.ext", L"z:\\path\\to\\filename" },
    test_extension{ L"z://path//to//filename.ext", L"z://path//to//filename" },
    test_extension{ L"z://path\\to//filename.ext", L"z://path\\to//filename" }
  ));

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
