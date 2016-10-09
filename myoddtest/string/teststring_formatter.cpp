#include "string/formatter.h"
#include "string/string.h"
#include "../testcommon.h"
#include <gtest/gtest.h>

TEST(StringTestFormater, WideStringAddLong)
{
  auto number = IntRandomNumber<long>();
  std::wstring result = ::myodd::strings::FormatterW() << L"Number :" << number;
  ASSERT_EQ(::myodd::strings::Format(L"Number :%d", number), result);
}

TEST(StringTestFormater, WideStringAddStrings)
{
  auto stringA = Uuid();
  auto stringB = Uuid();

  std::wstring result = ::myodd::strings::FormatterW() << stringA << stringB;
  ASSERT_EQ(::myodd::strings::Format(L"%s%s", stringA.c_str(), stringB.c_str()), result);
}

TEST(StringTestFormater, AsciiStringAddLong)
{
  auto number = IntRandomNumber<long>();
  std::string result = ::myodd::strings::FormatterA() << "Number :" << number;
  ASSERT_EQ(
    ::myodd::strings::WString2String( ::myodd::strings::Format( L"Number :%d", number) )
    , 
    result);
}

TEST(StringTestFormater, AsciiStringAddStrings)
{
  auto stringA = ::myodd::strings::WString2String( Uuid() );
  auto stringB = ::myodd::strings::WString2String( Uuid() );

  std::string result = ::myodd::strings::FormatterA() << stringA << stringB;
  ASSERT_EQ(
    ::myodd::strings::WString2String(
      ::myodd::strings::Format(L"%s%s", ::myodd::strings::String2WString(stringA).c_str(), ::myodd::strings::String2WString(stringB).c_str())
    )
    , 
    result);
}
