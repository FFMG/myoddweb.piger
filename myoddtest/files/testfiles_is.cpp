#include "string/string.h"
#include "files/files.h"
#include <gtest/gtest.h>

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

    test_istype{ L"://www.google.com", false },
    test_istype{ L"http://www.google.com:/efdere", false }, //  no port
    test_istype{ L"http://www.google.com:s/efdere", false }, //  bad port
    test_istype{ L"http://www.google.com:9s/efdere", false }, //  bad port
    test_istype{ L"http://www.google.com:8080/efdere", true } //  good port
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
  test_istype{ L"http://foo.bar?q=Spaces should be encoded", false },
  test_istype{ L"//", false },
  test_istype{ L"//a", false },
  test_istype{ L"///a", false },
  test_istype{ L"///", false },
  test_istype{ L"http:///a", false },
  test_istype{ L"foo.com", false },
  test_istype{ L"rdar ://1234", false },
  test_istype{ L"h://test", false },
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
