#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestCharacter* 
TEST(AnyTestCharacter, CheckTypeChar)
{
  char c = CharRandom<char>();
  auto x = myodd::dynamic::Any( c );
  ASSERT_EQ(myodd::dynamic::Type::Character_char, x.Type());
}

TEST(AnyTestCharacter, CheckTypeWChar)
{
  wchar_t c = CharRandom<wchar_t>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ(myodd::dynamic::Type::Character_wchar_t, x.Type());
}

TEST(AnyTestCharacter, CheckTypeUnsignedChar)
{
  unsigned char c = CharRandom<unsigned char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ(myodd::dynamic::Type::Character_unsigned_char, x.Type());
}

TEST(AnyTestCharacter, CheckTypeSignedChar)
{
  signed char c = CharRandom<signed char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ(myodd::dynamic::Type::Character_signed_char, x.Type());
}

TEST(AnyTestCharacter, CharToUnsignedChar)
{
  char c = CharRandom<char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ((unsigned char)c, (unsigned char)x);
}

TEST(AnyTestCharacter, UnsignedCharToChar)
{
  unsigned char c = CharRandom<unsigned char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ((char)c, (char)x);
}

TEST(AnyTestCharacter, CharToSignedChar)
{
  char c = CharRandom<char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ((signed char)c, (signed char)x);
}

TEST(AnyTestCharacter, UnsignedCharToSignedChar)
{
  unsigned char c = CharRandom<unsigned char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ((signed char)c, (signed char)x);
}

TEST(AnyTestCharacter, CharToLong)
{
  char c = CharRandom<char>();
  auto x = myodd::dynamic::Any(c);
  if (c >= '0' && c <= '9')
  {
    ASSERT_EQ((long)(c - '0'), (long)x);
  }
  else
  {
    ASSERT_EQ((long)0, (long)x);
  }
}

TEST(AnyTestCharacter, UnsignedCharToLong)
{
  unsigned char c = CharRandom<char>();
  auto x = myodd::dynamic::Any(c);
  if (c >= '0' && c <= '9')
  {
    ASSERT_EQ((long)(c - '0'), (long)x);
  }
  else
  {
    ASSERT_EQ((long)0, (long)x);
  }
}

TEST(AnyTestCharacter, CharToDouble)
{
  char c = CharRandom<char>();
  auto x = myodd::dynamic::Any(c);
  if (c >= '0' && c <= '9')
  {
    ASSERT_EQ((double)(c - '0'), (double)x);
  }
  else
  {
    ASSERT_EQ((double)0, (double)x);
  }
}

TEST(AnyTestCharacter, UnsignedCharToDouble)
{
  unsigned char c = CharRandom<unsigned char>();
  auto x = myodd::dynamic::Any(c);
  if (c >= '0' && c <= '9')
  {
    ASSERT_EQ((double)(c - '0'), (double)x);
  }
  else
  {
    ASSERT_EQ((double)0, (double)x);
  }
}

TEST(AnyTestCharacter, CharValueIskept)
{
  char c = CharRandom<char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ(c, x);
}

TEST(AnyTestCharacter, SignedCharValueIskept)
{
  signed char c = CharRandom<signed char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ(c, x);
}

TEST(AnyTestCharacter, UnsignedCharValueIskept)
{
  unsigned char c = CharRandom<unsigned char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ(c, x);
}

TEST(AnyTestCharacter, WCharValueIskept)
{
  wchar_t c = CharRandom<wchar_t>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ(c, x);
}

TEST(AnyTestCharacter, DefaultValueIsZeroChar )
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ( '\0', (char)x);
}

TEST(AnyTestCharacter, DefaultValueIsZeroUnsignedChar)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ('\0', (unsigned char)x);
}

TEST(AnyTestCharacter, DefaultValueIsZeroSignedChar)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ('\0', (signed char)x);
}

TEST(AnyTestCharacter, DefaultValueIsZeroWChar)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ( L'\0', (wchar_t)x );
}

TEST(AnyTestCharacter, VeryLargeNumbersCanStillBeCastBackToChar)
{
  //  get a large number
  unsigned long long int ulli = std::numeric_limits<unsigned long long int>::max();

  // create an any with it.
  auto x = myodd::dynamic::Any(ulli);

  // get the value as a char.
  // the actual value is implementation specific
  char c = x;
  ASSERT_TRUE(c >= std::numeric_limits<char>::min() && c <= std::numeric_limits<char>::max());
}

TEST(AnyTestCharacter, VeryLargeNumbersCanStillBeCastBackToWChar)
{
  //  get a large number
  unsigned long long int ulli = std::numeric_limits<unsigned long long int>::max();

  // create an any with it.
  auto x = myodd::dynamic::Any(ulli);

  // get the value as a wchar_t.
  // the actual value is implementation specific
  wchar_t c = x;
  ASSERT_TRUE(c >= std::numeric_limits<wchar_t>::min() && c <= std::numeric_limits<wchar_t>::max());
}

TEST(AnyTestCharacter, CharLetterEqualToZero)
{
  auto c = myodd::dynamic::Any('a');
  ASSERT_EQ(0, (int)c );
  ASSERT_EQ((float)0, (float)c);
  ASSERT_EQ((double)0, (double)c);
  ASSERT_EQ((long)0, (long)c);
  ASSERT_EQ('a', c);
}

TEST(AnyTestCharacter, WideCharLetterEqualToZero)
{
  auto c = myodd::dynamic::Any(L'a');
  ASSERT_EQ(0, (int)c);
  ASSERT_EQ((float)0, (float)c);
  ASSERT_EQ((double)0, (double)c);
  ASSERT_EQ((long)0, (long)c);
  ASSERT_EQ(L'a', c);
}

TEST(AnyTestCharacter, UnsignedCharLetterEqualToZero)
{
  auto c = myodd::dynamic::Any( unsigned char('a'));
  ASSERT_EQ(0, (int)c);
  ASSERT_EQ((float)0, (float)c);
  ASSERT_EQ((double)0, (double)c);
  ASSERT_EQ((long)0, (long)c);
  ASSERT_EQ(unsigned char('a'), c);
}

TEST(AnyTestCharacter, CastCharToWideChar)
{
  auto c = CharRandom<char>();
  auto x = myodd::dynamic::Any(c);
  auto w = (wchar_t)x;
  ASSERT_EQ((wchar_t)c, w);
}