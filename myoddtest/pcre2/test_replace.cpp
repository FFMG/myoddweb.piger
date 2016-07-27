#include <gtest/gtest.h>
#include <pcre2\regex2.h>

TEST(Pcre2TestsReplace, SingleLettersReplacement)
{
  // test a simple match
  const wchar_t* pattern = L"[a-z]";
  const wchar_t* replacement = L"1";
  std::wstring subjectResult = L"ABC";

  ASSERT_EQ(3, myodd::regex::Regex2::Replace(pattern, replacement, subjectResult, false));
  ASSERT_EQ( L"111", subjectResult);
}

TEST(Pcre2TestsReplace, SingleLettersReplacementCaseSensitive)
{
  // test a simple match
  const wchar_t* pattern = L"[a-z]";
  const wchar_t* replacement = L"1";
  std::wstring subjectResult = L"ABC";

  ASSERT_EQ(0, myodd::regex::Regex2::Replace(pattern, replacement, subjectResult, true));
  ASSERT_EQ(L"ABC", subjectResult);
}

TEST(Pcre2TestsReplace, ReplaceFindsNoMatch)
{
  // test a simple match
  const wchar_t* pattern = L"[a-z]";
  const wchar_t* replacement = L"x";
  std::wstring subjectResult = L"123-456";

  ASSERT_EQ(0, myodd::regex::Regex2::Replace(pattern, replacement, subjectResult, true));
  ASSERT_EQ(L"123-456", subjectResult);
}

TEST(Pcre2TestsReplace, SingleLettersReplacementCaseInsensitiveByCategory)
{
  // test a simple match
  const wchar_t* pattern = L"[\\p{L}]";
  const wchar_t* replacement = L"1";
  std::wstring subjectResult = L"ABC";

  ASSERT_EQ(3, myodd::regex::Regex2::Replace(pattern, replacement, subjectResult, true ));
  ASSERT_EQ(L"111", subjectResult);
}

TEST(Pcre2TestsReplace, ReplaceGroup)
{
  // test a simple match
  const wchar_t* pattern = L"\\p{L}+";
  const wchar_t* replacement = L"x";
  std::wstring subjectResult = L"ABC";

  ASSERT_EQ( 1, myodd::regex::Regex2::Replace(pattern, replacement, subjectResult, false ));
  ASSERT_EQ(L"x", subjectResult);
}

TEST(Pcre2TestsReplace, ReplaceUsingConstSubject)
{
  // test a simple match
  const wchar_t* pattern = L"\\p{L}+";
  const wchar_t* replacement = L"x";
  auto subject = L"ABC";
  std::wstring result = L"";

  ASSERT_EQ(1, myodd::regex::Regex2::Replace(pattern, replacement, subject, result, false));
  ASSERT_EQ(L"x", result);
}