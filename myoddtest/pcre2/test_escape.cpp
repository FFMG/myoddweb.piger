#include <gtest/gtest.h>
#include <pcre2\regex2.h>

TEST(Pcre2TestsEscape, SimpleEscape)
{
  // test a simple match
  const std::wstring subject = L".";

  auto result = myodd::regex::Regex2::Escape(subject);
  ASSERT_EQ( L"\\.", result);
}

TEST(Pcre2TestsEscape, SimpleEscapeOfTheEscape)
{
  // test a simple match
  const std::wstring subject = L"\\";

  auto result = myodd::regex::Regex2::Escape(subject);
  ASSERT_EQ(L"\\\\", result);
}

TEST(Pcre2TestsEscape, CompleteList)
{
  // test a simple match
  const std::wstring subject = L". \\ + * ? [ ^ ] $ ( ) { } = ! > < | : -':";

  auto result = myodd::regex::Regex2::Escape(subject);
  ASSERT_EQ(L"\\. \\\\ \\+ \\* \\? \\[ \\^ \\] \\$ \\( \\) \\{ \\} \\= \\! \\> \\< \\| \\: \\-\\'\\:", result);
}
