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

TEST(Pcre2TestsReplace, ReplaceWithPlaceHolder)
{
  // test a simple match
  const wchar_t* pattern = L"([a-z])";
  const wchar_t* replacement = L"$1-";
  std::wstring subjectResult = L"ABC";

  ASSERT_EQ(3, myodd::regex::Regex2::Replace(pattern, replacement, subjectResult, false));
  ASSERT_EQ(L"A-B-C-", subjectResult);
}

TEST(Pcre2TestsReplace, ReplaceWithMultiplePlaceHolders)
{
  // test a simple match
  const wchar_t* pattern = L"^([a-z]+) ([a-z]+), (.*), (.*), (.*), ([0-9]+)";
  const wchar_t* replacement = L"$6, $2, $1, $3, $4, $5";
  std::wstring subjectResult = L"John Smith, 385 Central Ave., Cincinnati, OH, 45238";

  ASSERT_EQ(1, myodd::regex::Regex2::Replace(pattern, replacement, subjectResult, false));
  ASSERT_EQ(L"45238, Smith, John, 385 Central Ave., Cincinnati, OH", subjectResult);
}

TEST(Pcre2TestsReplace, SearchAndReplaceDates)
{
  // test a simple match
  const wchar_t* pattern = L"(\\w+) (\\d+), (\\d+)";
  const wchar_t* replacement = L"${1}1,$3";
  std::wstring subjectResult = L"April 15, 2003";

  ASSERT_EQ(1, myodd::regex::Regex2::Replace(pattern, replacement, subjectResult, false));
  ASSERT_EQ(L"April1,2003", subjectResult);
}

TEST(Pcre2TestsReplace, StripMultipleWhiteSpaces)
{
  // test a simple match
  const wchar_t* pattern = L"\\s\\s+";
  const wchar_t* replacement = L" ";
  std::wstring subjectResult = L"foo   o";

  ASSERT_EQ(1, myodd::regex::Regex2::Replace(pattern, replacement, subjectResult, false));
  ASSERT_EQ(L"foo o", subjectResult);
}

TEST(Pcre2TestsReplace, MultipleNonMatchingWords)
{
  // test a simple match
  const wchar_t* pattern = L"(#\\w+)";
  const wchar_t* replacement = L"<a href='bla bla'>$1</a>";
  std::wstring subjectResult = L"any word here related to #English must #be replaced.";

  ASSERT_EQ(2, myodd::regex::Regex2::Replace(pattern, replacement, subjectResult, false));
  ASSERT_EQ(L"any word here related to <a href='bla bla'>#English</a> must <a href='bla bla'>#be</a> replaced.", subjectResult);
}
