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
