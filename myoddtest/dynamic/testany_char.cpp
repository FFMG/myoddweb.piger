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
  ASSERT_EQ((long)c, (long)x);
}

TEST(AnyTestCharacter, UnsignedCharToLong)
{
  unsigned char c = CharRandom<char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ((long)c, (long)x);
}

TEST(AnyTestCharacter, CharToDouble)
{
  char c = CharRandom<char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ((double)c, (double)x);
}

TEST(AnyTestCharacter, UnsignedCharToDouble)
{
  unsigned char c = CharRandom<unsigned char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ((double)c, (double)x);
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
