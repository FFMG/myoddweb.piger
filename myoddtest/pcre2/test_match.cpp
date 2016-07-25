#include <gtest/gtest.h>
#include <pcre2\regex2.h>

TEST(Pcre2Tests, SimpleTestCaseSensitive)
{
  // test a simple match
  const wchar_t *pattern = L"[a-z]";
  const wchar_t *subject = L"ABCabc";
  ASSERT_EQ( 6, myodd::regex::Regex2::Match(pattern, subject, false));
}

TEST(Pcre2Tests, SimpleTestCaseInSensitive)
{
  // test a simple match
  const wchar_t *pattern = L"[a-z]";
  const wchar_t *subject = L"ABCabc";
  ASSERT_EQ(3, myodd::regex::Regex2::Match(pattern, subject, true));
}

TEST(Pcre2Tests, SimpleTestCaseInSensitiveLettersCategories)
{
  // test a simple match
  const wchar_t *pattern = L"[\\p{L}]";
  const wchar_t *subject = L"ABCabc";

  // case sensitive is ignored because of \\p{L}
  ASSERT_EQ(6, myodd::regex::Regex2::Match(pattern, subject, true));
}

TEST(Pcre2Tests, SimpleTestLettersCategories)
{
  // test a simple match
  const wchar_t *pattern = L"[\\p{L}]";
  const wchar_t *subject = L"ABC";

  ASSERT_EQ(3, myodd::regex::Regex2::Match(pattern, subject, false));
}

TEST(Pcre2Tests, ZeroMatchAsNeeded)
{
  // test a simple match
  const wchar_t *pattern = L"(?:\\p{L})";
  const wchar_t *subject = L"ABC";

  ASSERT_EQ(3, myodd::regex::Regex2::Match(pattern, subject, false));
}

TEST(Pcre2Tests, MatchWithinMatch)
{
  // test a simple match
  const wchar_t *pattern = L"(Hel(lo))";
  const wchar_t *subject = L"Hello World";

  auto matches = myodd::regex::Regex2::matches();
  ASSERT_EQ(3, myodd::regex::Regex2::Match(pattern, subject, matches, false));
}

TEST(Pcre2Tests, MatchNextToAnotherMatch)
{
  // test a simple match
  const wchar_t *pattern = L"(He)llo (wo)rld";
  const wchar_t *subject = L"Hello World";

  auto matches = myodd::regex::Regex2::matches();
  ASSERT_EQ(3, myodd::regex::Regex2::Match(pattern, subject, matches, false));
  ASSERT_EQ(3, myodd::regex::Regex2::Match(pattern, subject, matches, false));
}
