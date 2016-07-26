#include <gtest/gtest.h>
#include <pcre2\regex2.h>

TEST(Pcre2TestsSearch, NullSubject)
{
  // test a simple match
  const wchar_t *pattern = L"[a-z]";
  const wchar_t *subject = nullptr;
  myodd::regex::Regex2::Matches m;
  ASSERT_EQ(PCRE2_ERROR_BADDATA, myodd::regex::Regex2::Match(pattern, subject, m, false));
}

TEST(Pcre2TestsSearch, NullPattern)
{
  // test a simple match
  const wchar_t *pattern = nullptr;
  const wchar_t *subject = L"Something";
  myodd::regex::Regex2::Matches m;
  ASSERT_EQ(PCRE2_ERROR_BADDATA, myodd::regex::Regex2::Match(pattern, subject, m, false));
}

TEST(Pcre2TestsSearch, NullPatternAndSubject)
{
  // test a simple match
  const wchar_t *pattern = nullptr;
  const wchar_t *subject = nullptr;
  myodd::regex::Regex2::Matches m;
  ASSERT_EQ(PCRE2_ERROR_BADDATA, myodd::regex::Regex2::Match(pattern, subject, m, false));
}

TEST(Pcre2TestsSearch, SimpleTestCaseSensitive)
{
  // test a simple match
  const wchar_t *pattern = L"[a-z]";
  const wchar_t *subject = L"ABCabc";
  myodd::regex::Regex2::Matches m;
  ASSERT_EQ(6, myodd::regex::Regex2::Match(pattern, subject, m, false));
}

TEST(Pcre2TestsSearch, SimpleTestCaseInSensitive)
{
  // test a simple match
  const wchar_t *pattern = L"[a-z]";
  const wchar_t *subject = L"ABCabc";
  myodd::regex::Regex2::Matches m;
  ASSERT_EQ(3, myodd::regex::Regex2::Match(pattern, subject, m, true));
}

TEST(Pcre2TestsSearch, SimpleTestCaseInSensitiveLettersCategories)
{
  // test a simple match
  const wchar_t *pattern = L"[\\p{L}]";
  const wchar_t *subject = L"ABCabc";

  // case sensitive is ignored because of \\p{L}
  myodd::regex::Regex2::Matches m;
  ASSERT_EQ(6, myodd::regex::Regex2::Match(pattern, subject, m, true));
}

TEST(Pcre2TestsSearch, SimpleTestLettersCategories)
{
  // test a simple match
  const wchar_t *pattern = L"[\\p{L}]";
  const wchar_t *subject = L"ABC";

  myodd::regex::Regex2::Matches m;
  ASSERT_EQ(3, myodd::regex::Regex2::Match(pattern, subject, m, false));
}

TEST(Pcre2TestsSearch, ZeroMatchAsNeeded)
{
  // test a simple match
  const wchar_t *pattern = L"(?:\\p{L})";
  const wchar_t *subject = L"ABC";

  myodd::regex::Regex2::Matches m;
  ASSERT_EQ(3, myodd::regex::Regex2::Match(pattern, subject, m, false));
}

TEST(Pcre2TestsSearch, MatchWithinMatch)
{
  // test a simple match
  const wchar_t *pattern = L"(Hel(lo))";
  const wchar_t *subject = L"Hello World";

  auto matches = myodd::regex::Regex2::Matches();
  ASSERT_EQ(3, myodd::regex::Regex2::Match(pattern, subject, matches, false));
}

TEST(Pcre2TestsSearch, MatchNextToAnotherMatch)
{
  // test a simple match
  const wchar_t *pattern = L"(He)llo (wo)rld";
  const wchar_t *subject = L"Hello World";

  auto matches = myodd::regex::Regex2::Matches();
  ASSERT_EQ(3, myodd::regex::Regex2::Match(pattern, subject, matches, false));
  ASSERT_EQ(matches[1], L"He");
  ASSERT_EQ(matches[2], L"Wo");
}

TEST(Pcre2TestsSearch, SearchUsingTheSameMatchVector)
{
  // repeat the search and make sure that the match contains are cleared.
  const wchar_t *pattern1 = L"(He)llo (wo)rld";
  const wchar_t *subject1 = L"Hello World";

  auto matches = myodd::regex::Regex2::Matches();

  ASSERT_EQ(3, myodd::regex::Regex2::Match(pattern1, subject1, matches, false));
  ASSERT_EQ(3, matches.size());
  ASSERT_EQ(matches[1], L"He");
  ASSERT_EQ(matches[2], L"Wo");

  const wchar_t *pattern2 = L"(Good)bye (wor)ld";
  const wchar_t *subject2 = L"Goodbye World";

  ASSERT_EQ(3, myodd::regex::Regex2::Match(pattern2, subject2, matches, false));
  ASSERT_EQ(3, matches.size());
  ASSERT_EQ(matches[1], L"Good");
  ASSERT_EQ(matches[2], L"Wor");
}
