#include <gtest/gtest.h>
#include <pcre2\regex2.h>

struct Pcre2Tests : testing::Test
{
public:
  myodd::regex::Regex2* _re;
  Pcre2Tests()
  {
    _re = new myodd::regex::Regex2();
  }

  ~Pcre2Tests()
  {
    delete _re;
  }
};

TEST_F(Pcre2Tests, SimpleTestCaseSensitive)
{
  // test a simple match
  const wchar_t *pattern = L"[a-z]";
  const wchar_t *subject = L"ABCabc";
  ASSERT_EQ( 6, _re->Match(pattern, subject, false));
}

TEST_F(Pcre2Tests, SimpleTestCaseInSensitive)
{
  // test a simple match
  const wchar_t *pattern = L"[a-z]";
  const wchar_t *subject = L"ABCabc";
  ASSERT_EQ(3, _re->Match(pattern, subject, true));
}

TEST_F(Pcre2Tests, SimpleTestCaseInSensitiveLettersCategories)
{
  // test a simple match
  const wchar_t *pattern = L"[\\p{L}]";
  const wchar_t *subject = L"ABCabc";

  // case sensitive is ignored because of \\p{L}
  ASSERT_EQ(6, _re->Match(pattern, subject, true));
}

TEST_F(Pcre2Tests, SimpleTestLettersCategories)
{
  // test a simple match
  const wchar_t *pattern = L"[\\p{L}]";
  const wchar_t *subject = L"ABC";
  ASSERT_EQ(3, _re->Match(pattern, subject, false));
}