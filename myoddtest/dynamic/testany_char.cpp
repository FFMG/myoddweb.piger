#include <gtest/gtest.h>
#include "../testcommon.h"

#include "dynamic/any.h"

// --gtest_filter=AnyTestCharacter* 
TEST_MEM(AnyTestCharacter, CheckTypeChar)
{
  char c = CharRandom<char>();
  auto x = myodd::dynamic::Any( c );
  ASSERT_EQ(myodd::dynamic::Type::Character_char, x.Type());
}

TEST_MEM(AnyTestCharacter, CheckTypeWChar)
{
  wchar_t c = CharRandom<wchar_t>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ(myodd::dynamic::Type::Character_wchar_t, x.Type());
}

TEST_MEM(AnyTestCharacter, CheckTypeUnsignedChar)
{
  unsigned char c = CharRandom<unsigned char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ(myodd::dynamic::Type::Character_unsigned_char, x.Type());
}

TEST_MEM(AnyTestCharacter, CheckTypeSignedChar)
{
  signed char c = CharRandom<signed char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ(myodd::dynamic::Type::Character_signed_char, x.Type());
}

TEST_MEM(AnyTestCharacter, CharToUnsignedChar)
{
  char c = CharRandom<char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ((unsigned char)c, (unsigned char)x);
}

TEST_MEM(AnyTestCharacter, UnsignedCharToChar)
{
  unsigned char c = CharRandom<unsigned char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ((char)c, (char)x);
}

TEST_MEM(AnyTestCharacter, CharToSignedChar)
{
  char c = CharRandom<char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ((signed char)c, (signed char)x);
}

TEST_MEM(AnyTestCharacter, UnsignedCharToSignedChar)
{
  unsigned char c = CharRandom<unsigned char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ((signed char)c, (signed char)x);
}

TEST_MEM(AnyTestCharacter, CharToLong)
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

TEST_MEM(AnyTestCharacter, UnsignedCharToLong)
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

TEST_MEM(AnyTestCharacter, CharToDouble)
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

TEST_MEM(AnyTestCharacter, UnsignedCharToDouble)
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

TEST_MEM(AnyTestCharacter, CharValueIskept)
{
  char c = CharRandom<char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ(c, x);
}

TEST_MEM(AnyTestCharacter, SignedCharValueIskept)
{
  signed char c = CharRandom<signed char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ(c, x);
}

TEST_MEM(AnyTestCharacter, UnsignedCharValueIskept)
{
  unsigned char c = CharRandom<unsigned char>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ(c, x);
}

TEST_MEM(AnyTestCharacter, WCharValueIskept)
{
  wchar_t c = CharRandom<wchar_t>();
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ(c, x);
}

TEST_MEM(AnyTestCharacter, DefaultValueIsZeroChar )
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ( '\0', (char)x);
}

TEST_MEM(AnyTestCharacter, DefaultValueIsZeroUnsignedChar)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ('\0', (unsigned char)x);
}

TEST_MEM(AnyTestCharacter, DefaultValueIsZeroSignedChar)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ('\0', (signed char)x);
}

TEST_MEM(AnyTestCharacter, DefaultValueIsZeroWChar)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ( L'\0', (wchar_t)x );
}

TEST_MEM(AnyTestCharacter, VeryLargeNumbersCanStillBeCastBackToChar)
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

TEST_MEM(AnyTestCharacter, VeryLargeNumbersCanStillBeCastBackToWChar)
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

TEST_MEM(AnyTestCharacter, CharLetterEqualToZero)
{
  auto c = myodd::dynamic::Any('a');
  ASSERT_EQ(0, (int)c );
  ASSERT_EQ((float)0, (float)c);
  ASSERT_EQ((double)0, (double)c);
  ASSERT_EQ((long)0, (long)c);
  ASSERT_EQ('a', c);
}

TEST_MEM(AnyTestCharacter, WideCharLetterEqualToZero)
{
  auto c = myodd::dynamic::Any(L'a');
  ASSERT_EQ(0, (int)c);
  ASSERT_EQ((float)0, (float)c);
  ASSERT_EQ((double)0, (double)c);
  ASSERT_EQ((long)0, (long)c);
  ASSERT_EQ(L'a', c);
}

TEST_MEM(AnyTestCharacter, UnsignedCharLetterEqualToZero)
{
  auto c = myodd::dynamic::Any( unsigned char('a'));
  ASSERT_EQ(0, (int)c);
  ASSERT_EQ((float)0, (float)c);
  ASSERT_EQ((double)0, (double)c);
  ASSERT_EQ((long)0, (long)c);
  ASSERT_EQ(unsigned char('a'), c);
}

TEST_MEM(AnyTestCharacter, CastCharToWideChar)
{
  auto c = CharRandom<char>();
  auto x = myodd::dynamic::Any(c);
  auto w = (wchar_t)x;
  ASSERT_EQ((wchar_t)c, w);
}

TEST_MEM(AnyTestCharacter, StringNumber)
{
  auto x = myodd::dynamic::Any("1234");
  ASSERT_EQ(1234, (int)x);
  ASSERT_EQ((double)1234, (double)x);
  ASSERT_EQ("1234", x);
}

TEST_MEM(AnyTestCharacter, WideStringNumber)
{
  auto x = myodd::dynamic::Any( L"1234");
  ASSERT_EQ(1234, (int)x);
  ASSERT_EQ((double)1234, (double)x);
  ASSERT_EQ( L"1234", x);
}

TEST_MEM(AnyTestCharacter, StringFloatingNumber)
{
  auto x = myodd::dynamic::Any("1234.567");
  ASSERT_EQ(1234, (int)x);
  ASSERT_EQ((double)1234.567, (double)x);
  ASSERT_EQ("1234.567", x);
}

TEST_MEM(AnyTestCharacter, WideStringFloatingNumber)
{
  auto x = myodd::dynamic::Any(L"1234.567");
  ASSERT_EQ(1234, (int)x);
  ASSERT_EQ((double)1234.567, (double)x);
  ASSERT_EQ(L"1234.567", x);
}

TEST_MEM(AnyTestCharacter, StringNotANumber)
{
  auto x = myodd::dynamic::Any("Hello");
  ASSERT_EQ(0, (int)x);
  ASSERT_EQ(0, (double)x);
  ASSERT_EQ("Hello", x);
  ASSERT_NE("World", x);
}

TEST_MEM(AnyTestCharacter, WideStringNotANumber)
{
  auto x = myodd::dynamic::Any(L"Hello");
  ASSERT_EQ(0, (int)x);
  ASSERT_EQ(0, (double)x);
  ASSERT_EQ(L"Hello", x);
  ASSERT_NE(L"World", x);
}

TEST_MEM(AnyTestCharacter, Compare2StringsSameValue)
{
  auto x = myodd::dynamic::Any("Hello");
  auto y = myodd::dynamic::Any("Hello");
  ASSERT_EQ(x, y);
}

TEST_MEM(AnyTestCharacter, Compare2WideStringsSameValue)
{
  auto x = myodd::dynamic::Any(L"Hello");
  auto y = myodd::dynamic::Any(L"Hello");
  ASSERT_EQ(x, y);
}

TEST_MEM(AnyTestCharacter, Compare2StringsSameValueButNotSameCase)
{
  auto x = myodd::dynamic::Any("HELLO");
  auto y = myodd::dynamic::Any("hello");
  ASSERT_NE(x, y);
}

TEST_MEM(AnyTestCharacter, Compare2WideStringsSameValueButNotSameCase)
{
  auto x = myodd::dynamic::Any(L"HELLO");
  auto y = myodd::dynamic::Any(L"hello");
  ASSERT_NE(x, y);
}

TEST_MEM(AnyTestCharacter, Compare2StringsSameLenghButNotSame)
{
  auto x = myodd::dynamic::Any("Hello");
  auto y = myodd::dynamic::Any("World");
  ASSERT_NE(x, y);
}

TEST_MEM(AnyTestCharacter, Compare2WideStringsSameLenghButNotSame)
{
  auto x = myodd::dynamic::Any(L"Hello");
  auto y = myodd::dynamic::Any(L"World");
  ASSERT_NE(x, y);
}

TEST_MEM(AnyTestCharacter, Compare2StringsString2AlmostSameButLonger)
{
  auto x = myodd::dynamic::Any("Hello");
  auto y = myodd::dynamic::Any("Hello2");
  ASSERT_NE(x, y);
}

TEST_MEM(AnyTestCharacter, Compare2WideStringsString2AlmostSameButLonger)
{
  auto x = myodd::dynamic::Any(L"Hello");
  auto y = myodd::dynamic::Any(L"Hello2");
  ASSERT_NE(x, y);
}

TEST_MEM(AnyTestCharacter, Compare2StringsString1AlmostSameButLonger)
{
  auto x = myodd::dynamic::Any("Hello2");
  auto y = myodd::dynamic::Any("Hello");
  ASSERT_NE(x, y);
}

TEST_MEM(AnyTestCharacter, Compare2StringsWideString1AlmostSameButLonger)
{
  auto x = myodd::dynamic::Any(L"Hello2");
  auto y = myodd::dynamic::Any(L"Hello");
  ASSERT_NE(x, y);
}

TEST_MEM(AnyTestCharacter, CompareANumberAndAString)
{
  auto x = myodd::dynamic::Any("1234");
  auto y = myodd::dynamic::Any(1234);
  ASSERT_EQ((int)x, y);
  ASSERT_EQ((const char*)y, x);
  ASSERT_EQ((char*)y, x);
}

TEST_MEM(AnyTestCharacter, CompareAFloatNumberAndAString)
{
  auto x = myodd::dynamic::Any("1234.456789");
  auto y = myodd::dynamic::Any(1234.456789);
  ASSERT_EQ((double)x, y);
  ASSERT_EQ((const char*)y, x);
  ASSERT_EQ((char*)y, x);
}

TEST_MEM(AnyTestCharacter, CompareANumberAndAWideString)
{
  auto x = myodd::dynamic::Any(L"1234");
  auto y = myodd::dynamic::Any(1234);
  ASSERT_EQ((int)x, y);
  ASSERT_EQ((const wchar_t*)y, x);
  ASSERT_EQ((wchar_t*)y, x);
}

TEST_MEM(AnyTestCharacter, CompareAFloatNumberAndAWideString)
{
  auto x = myodd::dynamic::Any(L"1234.456789");
  auto y = myodd::dynamic::Any(1234.456789);
  ASSERT_DOUBLE_EQ((double)x, y);
  ASSERT_EQ((const wchar_t*)y, x);
  ASSERT_EQ((wchar_t*)y, x);
}

TEST_MEM(AnyTestCharacter, CastFromWideCharToChar)
{
  auto x = myodd::dynamic::Any(L"1234.456789");
  ASSERT_STREQ((const char*)x, "1234.456789");
  ASSERT_STREQ((char*)x, "1234.456789");
}

TEST_MEM(AnyTestCharacter, CastFromNullWideCharToChar)
{
  auto x = myodd::dynamic::Any( (wchar_t*)nullptr );
  ASSERT_STREQ((const char*)x, "");
  ASSERT_STREQ((char*)x, "");
}

TEST_MEM(AnyTestCharacter, CastFromCharToWideChar)
{
  auto x = myodd::dynamic::Any("1234.456789");
  ASSERT_STREQ((wchar_t*)x, L"1234.456789");
  ASSERT_STREQ((wchar_t*)x, L"1234.456789");
}

TEST_MEM(AnyTestCharacter, CastFromNullCharToWideChar)
{
  auto x = myodd::dynamic::Any( (char*)nullptr );
  ASSERT_STREQ((wchar_t*)x, L"");
  ASSERT_STREQ((wchar_t*)x, L"");
}

TEST_MEM(AnyTestCharacter, StdStringConstructor )
{
  auto s = std::string("Hello");
  auto x = myodd::dynamic::Any(s);
  ASSERT_STREQ("Hello", x );
}

TEST_MEM(AnyTestCharacter, StdWideStringConstructor)
{
  auto s = std::wstring( L"Hello");
  auto x = myodd::dynamic::Any(s);
  ASSERT_STREQ( L"Hello", x);
}

TEST_MEM(AnyTestCharacter, StdWideStringPointerConstructor)
{
  auto s = std::wstring(L"Hello");
  auto x = myodd::dynamic::Any( &s );
  ASSERT_STREQ(L"Hello", x);
}

TEST_MEM(AnyTestCharacter, StdStringPointerConstructor)
{
  auto s = std::string("Hello");
  auto x = myodd::dynamic::Any(&s);
  ASSERT_STREQ(L"Hello", x);
}

TEST_MEM_LOOP(AnyTestCharacter, GetUnsignedCharPtrWithNoNullTerminatorInsideString, NUMBER_OF_TESTS)
{
  const size_t len = (size_t)CharRandom<unsigned char>();
  unsigned char* c = new unsigned char[len];
  memset(c, 0, len);
  for (auto i = 0; i < len-1; ++i)
  {
    auto x = CharRandom<unsigned char>();
    while (x == L'\0')
    {
      // we do not want a '\0'
      x = CharRandom<unsigned char>();
    }
    c[i] = x;
  }
  c[len - 1] = L'\0';
  
  auto any = myodd::dynamic::Any(c);
  auto cc = (unsigned char*)any;

  // we use expect so the memory is cleared
  // assert will kill the test right here.
  EXPECT_EQ( 0, memcmp(cc, c, len) );

  // clean up
  delete c;
}

TEST_MEM_LOOP(AnyTestCharacter, GetSignedCharPtrWithNoNullTerminatorInsideString, NUMBER_OF_TESTS)
{
  const size_t len = (size_t)CharRandom<unsigned char>();
  signed char* c = new signed char[len];
  memset(c, 0, len);
  for (auto i = 0; i < len; ++i)
  {
    auto x = CharRandom<signed char>();
    while (x == L'\0')
    {
      // we do not want a '\0'
      x = CharRandom<signed char>();
    }
    c[i] = x;
  }
  c[len - 1] = L'\0';

  auto any = myodd::dynamic::Any(c);
  auto cc = (signed char*)any;

  // we use expect so the memory is cleared
  // assert will kill the test right here.
  EXPECT_EQ(0, memcmp(cc, c, len));

  // clean up
  delete c;
}

TEST_MEM_LOOP(AnyTestCharacter, GetCharPtrWithNoNullTerminatorInsideString, NUMBER_OF_TESTS)
{
  const size_t len = (size_t)CharRandom<unsigned char>();
  char* c = new char[len];
  memset(c, 0, len);
  for (auto i = 0; i < len-1; ++i)
  {
    auto x = CharRandom<char>();
    while (x == '\0')
    {
      // we do not want a '\0'
      x = CharRandom<char>();
    }
    c[i] = x;
  }
  c[len-1] = '\0';

  auto any = myodd::dynamic::Any(c);
  auto cc = (char*)any;

  // we use expect so the memory is cleared
  // assert will kill the test right here.
  EXPECT_EQ(0, memcmp(cc, c, len));

  // clean up
  delete c;
}


TEST_MEM_LOOP(AnyTestCharacter, GetWideCharPtrWithNoNullTerminatorInsideString, NUMBER_OF_TESTS)
{
  const size_t len = (size_t)CharRandom<unsigned char>();
  wchar_t* c = new wchar_t[len];
  memset(c, 0, len);
  for (auto i = 0; i < len - 1; ++i)
  {
    auto x = CharRandom<wchar_t>();
    while (x == L'\0')
    {
      // we do not want a '\0'
      x = CharRandom<wchar_t>();
    }
    c[i] = x;
  }
  c[len - 1] = L'\0';

  auto any = myodd::dynamic::Any(c);
  auto cc = (wchar_t*)any;

  // we use expect so the memory is cleared
  // assert will kill the test right here.
  EXPECT_EQ(0, memcmp(cc, c, len*sizeof(wchar_t)));

  // clean up
  delete c;
}