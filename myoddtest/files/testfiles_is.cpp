#include "string/string.h"
#include "files/files.h"
#include <gtest/gtest.h>
#include "../testcommon.h"

const struct test_istype
{
  std::wstring given;
  bool is;

  friend std::ostream& operator <<(std::ostream& os, const test_istype& obj)
  {
    return os
      << "Given : '" << myodd::strings::WString2String(obj.given) << "'"
      << " Is : " << obj.is;
  }
};

struct MyOddFilesIsDot : testing::Test, testing::WithParamInterface<test_istype>
{
};

struct MyOddFilesIsUrl : testing::Test, testing::WithParamInterface<test_istype>
{
};

struct MyOddFilesIsDirectory : testing::Test, testing::WithParamInterface<test_istype>
{
};

TEST_P(MyOddFilesIsDot, TestIsDot)
{
  auto is = GetParam().is;
  auto given = GetParam().given;

  ASSERT_EQ(is, myodd::files::IsDot(given));
}

TEST_P(MyOddFilesIsUrl, TestIsUrl)
{
  auto is = GetParam().is;
  auto given = GetParam().given;

  ASSERT_EQ(is, myodd::files::IsURL(given));
}

TEST_P(MyOddFilesIsDirectory, TestIsDirectory)
{
  auto is = GetParam().is;
  auto given = GetParam().given;

  ASSERT_EQ(is, myodd::files::IsDirectory(given));
}

INSTANTIATE_TEST_CASE_P(TestIsDirectoryEnvironment, MyOddFilesIsDirectory,
  testing::Values(
    test_istype{ L"%appdata%", true },
    test_istype{ L"%ALLUSERSPROFILE%", true },
    test_istype{ L"%HOMEDRIVE%", true },
    test_istype{ L"%ProgramData%", true },
    test_istype{ L"%ProgramFiles%", true },
    test_istype{ L"%SYSTEMROOT%", true },
    test_istype{ L"%TEMP%", true },
    test_istype{ L"%TEMP%\\*.*", true },
    test_istype{ L"%TMP%", true },
    test_istype{ L"%USERPROFILE%", true }
  ));

INSTANTIATE_TEST_CASE_P(TestIsDot, MyOddFilesIsDot,
  testing::Values(
    test_istype{ L".", true },
    test_istype{ L"..", true },
    test_istype{ L"", false },
    test_istype{ L"\\.", true },
    test_istype{ L"\\..", true },
    test_istype{ L"/.", true },
    test_istype{ L"/..", true },
    test_istype{ L"c:\\example\\.", true }, // those values are still dots.
    test_istype{ L"c:\\example\\..", true },
    test_istype{ L"c:/example\\..", true },
    test_istype{ L"c:/example/..", true },
    test_istype{ L"c:/example\\..\\root\\", false },
    test_istype{ L"c:/example/../root/", false }
  ));

INSTANTIATE_TEST_CASE_P(TestIsUrl, MyOddFilesIsUrl,
  testing::Values(
    test_istype{ L"", false },
    test_istype{ L"          ", false },
    test_istype{ L"http://www.google.com", true },
    test_istype{ L"ftp://www.google.com", true },
    test_istype{ L"https://www.google.com", true },
    test_istype{ L"ftps://www.google.com", true },

    test_istype{ L"gopher://www.google.com", true },
    test_istype{ L"mailto://www.google.com", true },
    test_istype{ L"webcal://www.google.com", true },
    test_istype{ L"aim://www.google.com", true },
    test_istype{ L"prospero://www.google.com", true },
    test_istype{ L"file://www.google.com", true },
    test_istype{ L"wais://www.google.com", true },
    test_istype{ L"telnet://www.google.com", true },
    test_istype{ L"nntp://www.google.com", true },
    test_istype{ L"news://www.google.com", true },

    test_istype{ L"http://localhost", true },
    test_istype{ L"http://localhost", true },
    test_istype{ L"http://localhost:9090", true },
    test_istype{ L"h://test", true },
    test_istype{ L"http://&#x7D0D;&#x8C46;.example.org/%E2%80%AE", true },

    test_istype{ L"http://w/file.html?12=(123)#1222", true },

    test_istype{ L"://www.google.com", false },
    test_istype{ L"http://www.google.com:/efdere", false }, //  no port
    test_istype{ L"http://www.google.com:s/efdere", false }, //  bad port
    test_istype{ L"http://www.google.com:9s/efdere", false }, //  bad port
    test_istype{ L"http://www.google.com:8080/efdere", true } //  good port
  ));

//  http://formvalidation.io/validators/uri/
INSTANTIATE_TEST_CASE_P(TestValidUrls, MyOddFilesIsUrl,
  testing::Values(
    test_istype{ L"http://उदाहरण.परीक्षा", true },
    test_istype{ L"http://☺.damowmow.com/", true },
    test_istype{ L"http://➡.ws/䨹", true },
    test_istype{ L"http://⌘.ws", true },
    test_istype{ L"http://⌘.ws/", true },
    test_istype{ L"http://✪df.ws/123", true },
    test_istype{ L"http://foo.com/blah_blah", true },
    test_istype{ L"http://foo.com/blah_blah/", true },
    test_istype{ L"http://foo.com/blah_blah_(wikipedia)", true },
    test_istype{ L"http://foo.com/blah_blah_(wikipedia)_(again)", true },
    test_istype{ L"http://www.example.com/wpstyle/?p=364", true },
    test_istype{ L"https://www.example.com/foo/?bar=baz&inga=42&quux", true },
    test_istype{ L"http://userid:password@example.com:8080", true },
    test_istype{ L"http://userid:password@example.com:8080/", true },
    test_istype{ L"http://userid@example.com", true },
    test_istype{ L"http://userid@example.com/", true },
    test_istype{ L"http://userid@example.com:8080", true },
    test_istype{ L"http://userid@example.com:8080/", true },
    test_istype{ L"http://userid:password@example.com", true },
    test_istype{ L"http://userid:password@example.com/", true },
    test_istype{ L"http://142.42.1.1/", true },
    test_istype{ L"http://142.42.1.1:8080/", true },
    test_istype{ L"http://foo.com/blah_(wikipedia)#cite-1", true },
    test_istype{ L"http://foo.com/blah_(wikipedia)_blah#cite-1", true },
    test_istype{ L"http://foo.com/unicode_(✪)_in_parens", true },
    test_istype{ L"http://foo.com/(something)?after=parens", true },
    test_istype{ L"http://code.google.com/events/#&product=browser", true },
    test_istype{ L"http://j.mp", true },
    test_istype{ L"ftp://foo.bar/baz", true },
    test_istype{ L"http://foo.bar/?q=Test%20URL-encoded%20stuff", true },
    test_istype{ L"http://مثال.إختبار", true }, 	
    test_istype{ L"http://例子.测试", true },
    test_istype{ L"http://-.~_!$&'()*+,;=:%40:80%2f::::::@example.com", true },
    test_istype{ L"http://1337.net", true },
    test_istype{ L"http://a.b-c.de", true },
    test_istype{ L"http://223.255.255.254", true },
    test_istype{ L"ftp://foo:@host.com/", true },// null password is allowed.
    test_istype{ L"ftp://anonymous@ftp.mozilla.org/", true} // no password
  ));

//  http://formvalidation.io/validators/uri/
INSTANTIATE_TEST_CASE_P(TestInvalidUrls, MyOddFilesIsUrl,
  testing::Values(
  test_istype{ L"http://", false },
  test_istype{ L"http://.", false },
  test_istype{ L"http://..", false },
  test_istype{ L"http://../", false },
  test_istype{ L"http://?", false },
  test_istype{ L"http://??", false },
  test_istype{ L"http://??/", false },
  test_istype{ L"http://#", false },
  test_istype{ L"http://##", false },
  test_istype{ L"http://##/", false },
  test_istype{ L"http://foo.bar/spaces in the path should be encoded", false },
  test_istype{ L"http://foo.bar/?spaces in the path+query should be encoded", false },
  test_istype{ L"http://foo.bar?q=Spaces in the host+query should be encoded", false },
  test_istype{ L"http://foo.bar Spaces in the host should be encoded", false },
  test_istype{ L"//", false },
  test_istype{ L"//a", false },
  test_istype{ L"///a", false },
  test_istype{ L"///", false },
  test_istype{ L"http:///a", false },
  test_istype{ L"foo.com", false },
  test_istype{ L"rdar ://1234", false },
  test_istype{ L"http:// shouldfail.com", false },
  test_istype{ L":// should fail", false },
  test_istype{ L"http ://foo.bar/foo(bar)baz quux", false },
  test_istype{ L"ftps ://foo.bar/", false },
  test_istype{ L"http ://-error-.invalid/", false },
  test_istype{ L"http ://a.b--c.de/", false },
  test_istype{ L"http ://-a.b.co", false },
  test_istype{ L"http ://a.b-.co", false },
  test_istype{ L"http ://0.0.0.0", false },
  test_istype{ L"http ://10.1.1.0", false },
  test_istype{ L"http ://10.1.1.255", false },
  test_istype{ L"http ://224.1.1.1", false },
  test_istype{ L"http ://1.1.1.1.1", false },
  test_istype{ L"http ://123.123.123", false },
  test_istype{ L"http ://3628126748", false },
  test_istype{ L"http ://.www.foo.bar/", false },
  test_istype{ L"http ://www.foo.bar./", false },
  test_istype{ L"http ://.www.foo.bar./", false },
  test_istype{ L"http ://10.1.1.1", false },
  test_istype{ L"http ://10.1.1.254", false }
));

#include <fstream>
#include <iostream>

TEST(TestIsDirectory, TestTempDirectory)
{
  // get the temp directory
  std::wstring tempDirectory;
  if (!myodd::files::ExpandEnvironment(_T("%temp%"), tempDirectory))
  {
    FAIL();
  }

  // it is a directory
  ASSERT_TRUE(myodd::files::IsDirectory(tempDirectory));

  // but not a file.
  ASSERT_FALSE(myodd::files::IsFile(tempDirectory));

  std::wstring file;
  myodd::files::Join(file, tempDirectory, Uuid());

  // not yet a file
  ASSERT_FALSE(myodd::files::IsFile(file));

  // create it
  std::wofstream fs( file );
  if (!fs)
  {
    FAIL();
    return;
  }
  fs << "ghgh";
  fs.close();

  // it is now a file.
  ASSERT_TRUE(myodd::files::IsFile(file));

  // drop it
  ASSERT_TRUE(myodd::files::DeleteFile(file));

  // no longer a file.
  ASSERT_FALSE(myodd::files::IsFile(file));
}

TEST(TestIsDirectory, TestTempDirectoryExpanded)
{
  // get the temp directory
  std::wstring tempDirectory;
  if (!myodd::files::ExpandEnvironment(_T("%temp%"), tempDirectory))
  {
    FAIL();
  }

  // it is a directory
  ASSERT_TRUE(myodd::files::IsDirectory(_T("%temp%")));

  // but not a file.
  ASSERT_FALSE(myodd::files::IsFile(_T("%temp%")));

  auto uuid = Uuid();
  std::wstring file;
  myodd::files::Join(file, _T("%temp%"), uuid );

  // not yet a file
  ASSERT_FALSE(myodd::files::IsFile(file));

  std::wstring fileActual;
  myodd::files::Join(fileActual, tempDirectory, uuid );

  // create it
  std::wofstream fs(fileActual);
  if (!fs)
  {
    FAIL();
    return;
  }
  fs << "ghgh";
  fs.close();

  // it is now a file.
  ASSERT_TRUE(myodd::files::IsFile(file));

  // drop it
  ASSERT_TRUE(myodd::files::DeleteFile(file));

  // no longer a file.
  ASSERT_FALSE(myodd::files::IsFile(file));
  ASSERT_FALSE(myodd::files::IsFile(fileActual));
}