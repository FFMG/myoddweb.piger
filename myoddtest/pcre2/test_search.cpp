#include <gtest/gtest.h>
#include <pcre2\regex2.h>

TEST(Pcre2TestsSearch, NullSubject)
{
  // test a simple match
  const wchar_t *pattern = L"[a-z]";
  const wchar_t *subject = nullptr;
  ASSERT_FALSE(myodd::regex::Regex2::Search(pattern, subject, false));
}

TEST(Pcre2TestsSearch, NullPattern)
{
  // test a simple match
  const wchar_t *pattern = nullptr;
  const wchar_t *subject = L"Something";
  ASSERT_FALSE( myodd::regex::Regex2::Search(pattern, subject, false));
}

TEST(Pcre2TestsSearch, NullPatternAndSubject)
{
  // test a simple match
  const wchar_t *pattern = nullptr;
  const wchar_t *subject = nullptr;
  ASSERT_FALSE(myodd::regex::Regex2::Search(pattern, subject, false));
}

TEST(Pcre2TestsSearch, SimpleTestCaseSensitive)
{
  // test a simple match
  const wchar_t *pattern = L"[a-z]";
  const wchar_t *subject = L"ABCabc";
  ASSERT_TRUE(myodd::regex::Regex2::Search(pattern, subject, false));
}

TEST(Pcre2TestsSearch, SimpleTestCaseInSensitive)
{
  // test a simple match
  const wchar_t *pattern = L"[a-z]";
  const wchar_t *subject = L"ABCabc";
  ASSERT_TRUE(myodd::regex::Regex2::Search(pattern, subject, true));
}

TEST(Pcre2TestsSearch, SimpleTestCaseInSensitiveLettersCategories)
{
  // test a simple match
  const wchar_t *pattern = L"[\\p{L}]";
  const wchar_t *subject = L"ABCabc";

  // case sensitive is ignored because of \\p{L}
  ASSERT_TRUE(myodd::regex::Regex2::Search(pattern, subject, true));
}

TEST(Pcre2TestsSearch, SimpleTestLettersCategories)
{
  // test a simple match
  const wchar_t *pattern = L"[\\p{L}]";
  const wchar_t *subject = L"ABC";

  ASSERT_TRUE(myodd::regex::Regex2::Search(pattern, subject, false));
}

TEST(Pcre2TestsSearch, InSomeCasesSensitiveFlagDoesNotMatter)
{
  // test a simple match
  const wchar_t *pattern = L"(?:\\p{L})";
  const wchar_t *subject = L"ABC";

  ASSERT_TRUE( myodd::regex::Regex2::Search(pattern, subject, false));
  ASSERT_TRUE(myodd::regex::Regex2::Search(pattern, subject, true));
}

TEST(Pcre2TestsSearch, ZeroMatchesRegadlessCaseLowerCase)
{
  // we only want lower case
  const wchar_t *pattern = L"(?:\\p{Ll})";
  const wchar_t *subject = L"ABC";

  ASSERT_FALSE( myodd::regex::Regex2::Search(pattern, subject, false));
  ASSERT_FALSE( myodd::regex::Regex2::Search(pattern, subject, true));
}

TEST(Pcre2TestsSearch, MatchWithinMatch)
{
  // test a simple match
  const wchar_t *pattern = L"(Hel(lo))";
  const wchar_t *subject = L"Hello World";

  ASSERT_TRUE( myodd::regex::Regex2::Search(pattern, subject, false));
}

TEST(Pcre2TestsSearch, MatchNextToAnotherMatch)
{
  // test a simple match
  const wchar_t *pattern = L"(He)llo (wo)rld";
  const wchar_t *subject = L"Hello World";

  ASSERT_TRUE(myodd::regex::Regex2::Search(pattern, subject, false));
}
