#include "config/data.h"
#include "string\string.h"
#include "string\formatter.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=ConfigDataTest* 

TEST_MEM(ConfigDataTest, DefaultConstructor)
{
  // no error should happen
  ::myodd::config::Data data;
}

TEST_MEM(ConfigDataTest, SetValueWithoutLoadingAnXML)
{
  // no error should happen
  ::myodd::config::Data data;
  data.Set(XmlElementName(), Uuid(), true );
}

TEST_MEM(ConfigDataTest, LoadFromAnXML)
{
  ::myodd::config::Data data;
  ASSERT_TRUE( data.LoadXml(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><value type=\"8\">42</value></Config>") );
}

TEST_MEM(ConfigDataTest, LoadFromAnXMLAndGetTheValueSingleDepth)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format( L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><value type=\"8\">%d</value></Config>", number );
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get( L"value"));
}

TEST_MEM(ConfigDataTest, LoadFromAnXMLAndGetTheValueDoubleDepth)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get(L"parent\\value"));
}

TEST_MEM(ConfigDataTest, LoadFromAnXMLAndGetTheValueDoubleDepthDifferentGetCases)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get(L"parent\\value"));
  ASSERT_EQ(number, data.Get(L"parent\\VALUE"));
  ASSERT_EQ(number, data.Get(L"Parent\\Value"));
}

TEST_MEM(ConfigDataTest, LoadFromAnXMLAndGetTheValueDoubleDepthSourceHasDifferentCase)
{
  // source has upper case 
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><Parent><Value type=\"8\">%d</Value></Parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get(L"parent\\value"));
  ASSERT_EQ(number, data.Get(L"parent\\VALUE"));
  ASSERT_EQ(number, data.Get(L"Parent\\Value"));
}

TEST_MEM(ConfigDataTest, GetAValueThatDoesNotExist)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get(L"parent\\value"));
  EXPECT_THROW(data.Get(L"something\\else"), std::exception);
}

TEST_MEM(ConfigDataTest, GetAValueWithSpaces)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get(L"     parent\\value    "));
}

TEST_MEM(ConfigDataTest, GetAValueWithExtrabackslashes)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get(L"\\parent\\value\\"));
}

TEST_MEM(ConfigDataTest, GetAValueWithMultiplebackslashes)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get(L"\\\\parent\\\\\\value\\\\\\"));
}

TEST_MEM(ConfigDataTest, NumersAndMinusAreValidElementNames)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><Value-123 type=\"8\">%d</Value-123></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get(L"parent\\Value-123"));
}

TEST_MEM(ConfigDataTest, NumersAreValidElementNames)
{
  //  invalid XML, (just number <123>sss</123> will not be accepted at load time)
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><x123 type=\"8\">%d</x123></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get(L"parent\\x123"));
}

TEST_MEM(ConfigDataTest, GetAValueFromEmptyData)
{
  ::myodd::config::Data data;
  EXPECT_THROW(data.Get(L"something\\else"), std::exception);
}

TEST_MEM(ConfigDataTest, CheckIfContainsTheValue)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_TRUE( data.Contains(L"parent\\value"));
}

TEST_MEM(ConfigDataTest, CheckIfDoesNotConainContainsTheValueSimilarName)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_TRUE(data.Contains(L"parent\\value"));

  // one extra char
  ASSERT_FALSE(data.Contains(L"parent\\value1"));

  // one less char
  ASSERT_FALSE(data.Contains(L"parent\\valu"));
}

TEST_MEM(ConfigDataTest, CheckIfDoesNotConainContainsTheValueWrongParent)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_TRUE(data.Contains(L"parent\\value"));

  // one extra char
  ASSERT_FALSE(data.Contains(L"parentt\\value"));

  // one less char
  ASSERT_FALSE(data.Contains(L"paren\\value"));
}

TEST_MEM(ConfigDataTest, ContainsWithBadNameWillThrowWithNumber)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  EXPECT_THROW(data.Contains(L"123Bad"), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, ContainsWithBadNameWillThrowWithSpaces)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  EXPECT_THROW(data.Contains(L"Bad Name"), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, ContainsWithBadNameWillThrowWithInvalidCharacters)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  EXPECT_THROW(data.Contains(L"Bad+Name"), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, CheckIfContainsTheValueVariousCase)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_TRUE(data.Contains(L"Parent\\Value"));
  ASSERT_TRUE(data.Contains(L"Parent\\VALUE"));
  ASSERT_TRUE(data.Contains(L"PAREnt\\Value"));
}

TEST_MEM(ConfigDataTest, GetDefaultValueForNonExistent)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));

  auto defaultNumber = IntRandomNumber<int>();
  ASSERT_EQ(defaultNumber, data.Get(L"some\\other\\value", defaultNumber));
}

TEST_MEM(ConfigDataTest, GetDefaultValueForNonExistentButAlmostValid)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));

  auto defaultNumber = IntRandomNumber<int>();
  // almost the same as parent\\value
  ASSERT_EQ(defaultNumber, data.Get(L"parent\\value1", defaultNumber));
}

TEST_MEM(ConfigDataTest, GetDefaultValueButValueExists)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));

  // the value does exist
  auto defaultNumber = IntRandomNumber<int>();
  ASSERT_EQ(number, data.Get(L"parent\\value", defaultNumber));
}

TEST_MEM(ConfigDataTest, GetWithSpaces)
{
  ::myodd::config::Data data;
  EXPECT_THROW(data.Get(L"    "), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, GetWithNoSpaces)
{
  ::myodd::config::Data data;
  EXPECT_THROW(data.Get(L""), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, GetWithOnlyBackSpace)
{
  ::myodd::config::Data data;
  EXPECT_THROW(data.Get(L"\\"), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, YouCannotSetANameWithANumberOnly)
{
  // those are not valid xml element names
  ::myodd::config::Data data;
  EXPECT_THROW(data.Set(L"1234", 1234, false), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, YouCannotSetANameWithANumberEvenWithMinus)
{
  // those are not valid xml element names
  ::myodd::config::Data data;
  EXPECT_THROW(data.Set(L"1234-5678", 1234, false), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, YouCannotSetANameWithANumberOnlyEvenInThePath)
{
  // those are not valid xml element names
  ::myodd::config::Data data;
  EXPECT_THROW(data.Set(L"valid\\path\\1234", 1234, false), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, YouCannotSetAnEllementThatStartsWithAMinus)
{
  // those are not valid xml element names
  ::myodd::config::Data data;
  data.Set(L"valid\\path\\good-name", 1234, false);
  EXPECT_THROW(data.Set(L"valid\\path\\-bad", 1234, false), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, YouCannotSetANameWithANumberWithMinusOnlyEvenInThePath)
{
  // those are not valid xml element names
  ::myodd::config::Data data;
  EXPECT_THROW(data.Set(L"valid\\path\\1234-5678", 1234, false), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, YouCannotSetANameWithANumberOnlyAtTheBeginingEvenInThePath)
{
  // those are not valid xml element names
  ::myodd::config::Data data;
  EXPECT_THROW(data.Set(L"1234\\valid\\path", 1234, false), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, WhenCheckingForTempTheNameMustStillBeValidNumber)
{
  // those are not valid xml element names
  ::myodd::config::Data data;
  EXPECT_THROW(data.IsTemp(L"1234"), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, WhenCheckingForTempTheNameMustStillBeValidNumberStartPath)
{
  // those are not valid xml element names
  ::myodd::config::Data data;
  EXPECT_THROW(data.IsTemp(L"1234\\valid\\path"), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, AValueThatDoesNotExistIsNotTemp)
{
  // those are not valid xml element names
  ::myodd::config::Data data;
  ASSERT_FALSE(data.IsTemp(L"\\valid\\path"));
}

TEST_MEM(ConfigDataTest, SwitchBetweenTempAndNotTemp)
{
  // those are not valid xml element names
  ::myodd::config::Data data;
  auto name = L"\\valid\\path";
  auto number = IntRandomNumber<int>();
  ASSERT_FALSE(data.IsTemp(name));
  data.Set(name, number, false);
  ASSERT_FALSE(data.IsTemp(name));
  
  // make it temp
  data.Set(name, number, true);
  ASSERT_TRUE(data.IsTemp(name));

  // make it permanent
  data.Set(name, number, false);
  ASSERT_FALSE(data.IsTemp(name));
}

TEST_MEM(ConfigDataTest, LoadedValuesAreNotTempByDefault)
{
  ::myodd::config::Data data;
  auto number1 = IntRandomNumber<int>();
  auto value1 = ::myodd::strings::Format(L"<value1 type = \"8\">%d</value1>", number1);
  auto number2 = IntRandomNumber<int>();
  auto value2 = ::myodd::strings::Format(L"<value2 type = \"8\">%d</value2>", number2);

  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent>%s%s</parent></Config>", value1.c_str(), value2.c_str());
  ASSERT_TRUE(data.LoadXml(source));

  // they all temp by default
  ASSERT_FALSE(data.IsTemp(L"value1"));
  ASSERT_FALSE(data.IsTemp(L"value2"));

  // set #1 to temp
  data.Set(L"value1", number1, true);
  ASSERT_TRUE(data.IsTemp(L"value1"));

  // the other is still not temp.
  ASSERT_FALSE(data.IsTemp(L"value2"));
}

TEST_MEM(ConfigDataTest, CheckStringGetSetVaues)
{
  ::myodd::config::Data data;
  auto name = L"\\valid\\path";
  auto set = Uuid();
  data.Set(name, set, true );
  
  std::wstring get = data.Get(name);
  ASSERT_EQ(set, get);
}

TEST_MEM(ConfigDataTest, GetSetVauesAsConstChar)
{
  ::myodd::config::Data data;
  auto name = L"\\valid\\path";
  auto set = Uuid();
  data.Set(name, set, true);
  ASSERT_STREQ(set.c_str(), data.Get(name));
}

TEST_MEM(ConfigDataTest, CheckStringGetSetVauesAsConstChar)
{
  ::myodd::config::Data data;
  auto name = L"\\valid\\path";
  auto set = Uuid();
  data.Set(name, set, true);
  ASSERT_EQ(set, data.Get(name));
}

TEST_MEM(ConfigDataTest, BaseNameCannotHaveSpaces )
{
  EXPECT_THROW( ::myodd::config::Data data( L"  Bad  "), std::invalid_argument );
}

TEST_MEM(ConfigDataTest, BaseNameCannotHaveSpacesBetweenCharacters)
{
  EXPECT_THROW(::myodd::config::Data data(L"Bad Name"), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, BaseNameCannotHaveSpecialCharacters)
{
  EXPECT_THROW(::myodd::config::Data data(L"Bad+Name"), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, BaseNameCannotHaveBackSlashInTheMiddle)
{
  EXPECT_THROW(::myodd::config::Data data(L"Bad\\Name"), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, BaseNameCannotStartWithNumber)
{
  EXPECT_THROW(::myodd::config::Data data(L"12BadName"), std::invalid_argument);
}

TEST_MEM_LOOP(ConfigDataTest, LoadFromAnXMLAndGetTheValueDoubleDepthWithNonDefaultName, NUMBER_OF_TESTS)
{
  auto element = XmlElementName();
  ::myodd::config::Data data( element );
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><%s><parent><value type=\"8\">%d</value></parent></%s>", element.c_str(), number, element.c_str());
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get(L"parent\\value"));
}

TEST_MEM_LOOP(ConfigDataTest, NegativeVersionNumber, NUMBER_OF_TESTS)
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  auto version = IntRandomNumber<long>(-100, -1);
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><%s version=\"%d\"></%s>", element.c_str(), version, element.c_str());
  EXPECT_THROW(data.LoadXml(source), std::exception );
}

TEST_MEM(ConfigDataTest, PreviousVersionNumberString)
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  auto type = 1;
  std::wstring source = ::myodd::strings::Formatter() << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><" << element << "><value type=\"" << type << L"\">Hello</value></" << element << L">";
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(L"Hello", data.Get(L"value"));
  ASSERT_EQ(::myodd::dynamic::Type::Character_wchar_t, data.Get(L"value").Type());
}

TEST_MEM(ConfigDataTest, PreviousVersionNumberLongInt)
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  auto number = IntRandomNumber<long>();
  auto type = 2;
  std::wstring source = ::myodd::strings::Formatter() << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><" << element << "><value type=\"" << type << L"\">" << number << L"</value></" << element << L">";
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get(L"value"));
  ASSERT_EQ(::myodd::dynamic::Type::Integer_long_long_int, data.Get(L"value").Type());
}

TEST_MEM(ConfigDataTest, PreviousVersionNumberDouble)
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  auto number = RealRandomNumber<double>();
  auto type = 4;
  std::wstring source = ::myodd::strings::Formatter() << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><" << element << "><value type=\"" << type << L"\">" << std::fixed << number << L"</value></" << element << L">";
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_DOUBLE_EQ(number, data.Get(L"value"));
  ASSERT_EQ(::myodd::dynamic::Type::Floating_point_long_double, data.Get(L"value").Type());
}

TEST_MEM(ConfigDataTest, PreviousVersionNumberInt)
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  auto number = IntRandomNumber<int>();
  auto type = 8;
  std::wstring source = ::myodd::strings::Formatter() << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><" << element << "><value type=\"" << type << L"\">" << number << L"</value></" << element << L">";
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get(L"value"));
  ASSERT_EQ(::myodd::dynamic::Type::Integer_long_long_int, data.Get(L"value").Type());
}

TEST_MEM(ConfigDataTest, PreviousVersionNumberLongLongInt)
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  auto number = IntRandomNumber<long long int>();
  auto type = 16;
  std::wstring source = ::myodd::strings::Formatter() << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><" << element << "><value type=\"" << type << L"\">" << number << L"</value></" << element << L">";
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get(L"value"));
  ASSERT_EQ(::myodd::dynamic::Type::Integer_long_long_int, data.Get(L"value").Type());
}

TEST_MEM(ConfigDataTest, CurrentVersionNumberString)
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  auto version = 1;
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><%s version=\"%d\"><value type=\"8\">Hello</value></%s>", element.c_str(), version, element.c_str());
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(L"Hello", data.Get(L"value"));
  ASSERT_EQ(::myodd::dynamic::Type::Character_wchar_t, data.Get(L"value").Type());
}

TEST_MEM_LOOP(ConfigDataTest, CurrentVersionNumberBoolean, NUMBER_OF_TESTS )
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  auto version = 1;
  auto boolean = BoolRandomNumber();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><%s version=\"%d\"><value type=\"4\">%d</value></%s>", element.c_str(), version,(boolean?1:0), element.c_str());
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(boolean, data.Get(L"value"));
  ASSERT_EQ(::myodd::dynamic::Type::Boolean_bool, data.Get(L"value").Type());
}

TEST_MEM(ConfigDataTest, CurrentVersionNumberLongInt)
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  auto version = 1;
  auto number = IntRandomNumber<long>();
  auto type = 13;
  std::wstring source = ::myodd::strings::Formatter() << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><" << element << " version=\"" << version << L"\"><value type=\"" << type << L"\">"<< number << L"</value></" << element << L">";
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get(L"value"));
  ASSERT_EQ(::myodd::dynamic::Type::Integer_long_long_int, data.Get(L"value").Type());
}

TEST_MEM(ConfigDataTest, CurrentVersionNumberDouble)
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  auto version = 1;
  auto number = RealRandomNumber<double>();
  auto type = 18;
  std::wstring source = ::myodd::strings::Formatter() << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><" << element << " version=\"" << version << L"\"><value type=\"" << type << L"\">" << std::fixed << number << L"</value></" << element << L">";
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_DOUBLE_EQ(number, data.Get(L"value"));
  ASSERT_EQ(::myodd::dynamic::Type::Floating_point_long_double, data.Get(L"value").Type());
}

TEST_MEM(ConfigDataTest, CurrentVersionNumberInt)
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  auto version = 1;
  auto number = IntRandomNumber<int>();
  auto type = 11;
  std::wstring source = ::myodd::strings::Formatter() << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><" << element << " version=\"" << version << L"\"><value type=\"" << type << L"\">" << number << L"</value></" << element << L">";
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get(L"value"));
  ASSERT_EQ(::myodd::dynamic::Type::Integer_long_long_int, data.Get(L"value").Type());
}

TEST_MEM(ConfigDataTest, CurrentVersionNumberLongLongInt)
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  auto version = 1;
  auto number = IntRandomNumber<long long int>();
  auto type = 15;
  std::wstring source = ::myodd::strings::Formatter() << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><" << element << " version=\"" << version << L"\"><value type=\"" << type << L"\">" << number << L"</value></" << element << L">";
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_EQ(number, data.Get(L"value"));
  ASSERT_EQ(::myodd::dynamic::Type::Integer_long_long_int, data.Get(L"value").Type());
}

TEST_MEM(ConfigDataTest, CurrentVersionNumberUnknownType)
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  auto version = 1;
  auto number = IntRandomNumber<long long int>();
  auto type = 0;
  std::wstring source = ::myodd::strings::Formatter() << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><" << element << " version=\"" << version << L"\"><value type=\"" << type << L"\">" << number << L"</value></" << element << L">";
  EXPECT_THROW(data.LoadXml(source), std::runtime_error );
}

TEST_MEM(ConfigDataTest, PreviousVersionNumberUnknownType)
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  auto number = IntRandomNumber<long long int>();
  auto type = 17;
  std::wstring source = ::myodd::strings::Formatter() << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><" << element << "><value type=\"" << type << L"\">" << number << L"</value></" << element << L">";
  EXPECT_THROW(data.LoadXml(source), std::runtime_error);
}

TEST_MEM_LOOP(ConfigDataTest, ChangeOnlyTheValueNotTemp, NUMBER_OF_TESTS )
{
  ::myodd::config::Data data;
  auto element = XmlElementName();
  auto temp = BoolRandomNumber();
  auto uuid1 = Uuid();
  auto uuid2 = Uuid();

  data.Set(element, uuid1, temp );
  ASSERT_EQ(uuid1, data.Get(element));
  ASSERT_EQ(temp, data.IsTemp(element));

  //  only change the value
  data.Set(element, uuid2, temp);
  ASSERT_EQ(uuid2, data.Get(element));
  ASSERT_EQ(temp, data.IsTemp(element));
}

TEST_MEM_LOOP(ConfigDataTest, ChangeNothingAtAll, NUMBER_OF_TESTS)
{
  ::myodd::config::Data data;
  auto element = XmlElementName();
  auto temp = BoolRandomNumber();
  auto uuid = Uuid();

  data.Set(element, uuid, temp);
  ASSERT_EQ(uuid, data.Get(element));
  ASSERT_EQ(temp, data.IsTemp(element));

  // change nothing
  data.Set(element, uuid, temp);
  ASSERT_EQ(uuid, data.Get(element));
  ASSERT_EQ(temp, data.IsTemp(element));
}

TEST_MEM(ConfigDataTest, InvalidXmlWillReturnFalse )
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  auto source = L"!Invalid XML";
  ASSERT_FALSE(data.LoadXml(source));
}

TEST_MEM(ConfigDataTest, ValidXMLWithoutThExpectedRoot)
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  auto number = IntRandomNumber<int>();

  auto elementOther = XmlElementName();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><%s><parent><value type=\"8\">%d</value></parent></%s>", elementOther.c_str(), number, elementOther.c_str());
  ASSERT_FALSE(data.LoadXml(source));
}

TEST_MEM(ConfigDataTest, CreateASimpleXmlWithDefaultRoot)
{
  ::myodd::config::Data data;
  data.Set(L"path", L"Hello", false);

  auto xml = data.SaveXml();

  std::wstring expected = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Config version=\"1\">\n    <path type=\"8\">Hello</path>\n</Config>\n";
  ASSERT_EQ(expected, xml);
}

TEST_MEM_LOOP(ConfigDataTest, CreateASimpleXmlWithDefaultRootWithBoolean, NUMBER_OF_TESTS)
{
  ::myodd::config::Data data;
  auto boolean = BoolRandomNumber();
  data.Set(L"path", boolean, false);

  auto xml = data.SaveXml();

  std::wstring expected = ::myodd::strings::FormatterW() << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Config version=\"1\">\n    <path type=\"4\">" << (boolean? L"1" : L"0") << L"</path>\n</Config>\n";
  ASSERT_EQ(expected, xml);
}

TEST_MEM(ConfigDataTest, CreateASimpleXmlWithDefaultRootWithLongLongInt)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<long long int>();
  data.Set(L"path", number, false);

  auto xml = data.SaveXml();

  std::wstring expected = ::myodd::strings::FormatterW() << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Config version=\"1\">\n    <path type=\"15\">" << number << L"</path>\n</Config>\n";
  ASSERT_EQ(expected, xml);
}

TEST_MEM(ConfigDataTest, CreateASimpleXmlWithDefaultRootWithLongDouble)
{
  ::myodd::config::Data data;
  auto number = RealRandomNumber<double>();
  data.Set(L"path", number, false);

  auto xml = data.SaveXml();

  std::wstring expected = ::myodd::strings::FormatterW() << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Config version=\"1\">\n    <path type=\"18\">" << myodd::strings::ToString((long double)number) << L"</path>\n</Config>\n";
  ASSERT_EQ(expected, xml);
}

TEST_MEM(ConfigDataTest, CreateASimpleXmlWithDefaultRootWithLongLongIntMultipleEntries)
{
  ::myodd::config::Data data;
  auto number1 = IntRandomNumber<long long int>();
  data.Set(L"path1", number1, false);

  auto number2 = IntRandomNumber<long long int>();
  data.Set(L"path2", number2, false);

  auto xml = data.SaveXml();

  std::wstring expected = ::myodd::strings::FormatterW() << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Config version=\"1\">\n    <path1 type=\"15\">" << number1 << L"</path1>\n    <path2 type=\"15\">" << number2 << L"</path2>\n</Config>\n";
  ASSERT_EQ(expected, xml);
}

TEST_MEM(ConfigDataTest, CreateASimpleXmlWithDefaultRootWithLongLongIntMultipleEntriesAndTemp)
{
  ::myodd::config::Data data;
  auto number1 = IntRandomNumber<long long int>();
  data.Set(L"path1", number1, false);

  auto number2 = IntRandomNumber<long long int>();
  data.Set(L"path2", number2, false);

  // the temp value is not added.
  auto number3 = IntRandomNumber<long long int>();
  data.Set(L"path3", number2, true);

  auto xml = data.SaveXml();

  std::wstring expected = ::myodd::strings::FormatterW() << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Config version=\"1\">\n    <path1 type=\"15\">" << number1 << L"</path1>\n    <path2 type=\"15\">" << number2 << L"</path2>\n</Config>\n";
  ASSERT_EQ(expected, xml);
}

TEST_MEM(ConfigDataTest, CreateASimpleXmlWithNonDefaultRoot)
{
  auto element = XmlElementName();
  ::myodd::config::Data data(element);
  data.Set(L"path", L"Hello", false);

  auto xml = data.SaveXml();

  std::wstring expected = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<%s version=\"1\">\n    <path type=\"8\">Hello</path>\n</%s>\n";

  auto x = ::myodd::strings::Format(expected.c_str(), element.c_str(), element.c_str());
  ASSERT_EQ(x, xml);
}

TEST_MEM(ConfigDataTest, CannotSetAnObjectAsConfigValue)
{
  struct X {
    std::string _x;
  };
  X x;
  ::myodd::config::Data data;
  EXPECT_THROW( data.Set(L"path", x, false), std::runtime_error );
}

TEST_MEM(ConfigDataTest, NotDirtyAfterLoading)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config version=\"1\"><value type=\"11\">%d</value></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_FALSE(data.IsDirty());
}

TEST_MEM(ConfigDataTest, NotDirtyAfterNotReallyChangingTheValueLongLongInt)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config version=\"1\"><value type=\"11\">%d</value></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_FALSE(data.IsDirty());

  // set the value to the same value
  data.Set(L"value", number, false);

  // nothing should have changed.
  ASSERT_FALSE(data.IsDirty());
}

TEST_MEM(ConfigDataTest, NotDirtyAfterNotReallyChangingTheValueString)
{
  ::myodd::config::Data data;
  auto stringValue = L"Hello";
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config version=\"1\"><value type=\"8\">%s</value></Config>", stringValue);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_FALSE(data.IsDirty());

  // set the value to the same value
  data.Set(L"value", stringValue, false);

  // nothing should have changed.
  ASSERT_FALSE(data.IsDirty());
}

TEST_MEM(ConfigDataTest, NotDirtyAfterNotReallyChangingTheValueFloat)
{
  ::myodd::config::Data data;
  auto number = RealRandomNumber<float>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config version=\"1\"><value type=\"18\">%.64lf</value></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_FALSE(data.IsDirty());

  // set the value to the same value
  data.Set(L"value", number, false);

  // nothing should have changed.
  ASSERT_FALSE(data.IsDirty());
}

TEST_MEM(ConfigDataTest, NotDirtyAfterNotReallyChangingTheValueBoolean)
{
  ::myodd::config::Data data;
  auto number = BoolRandomNumber();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config version=\"1\"><value type=\"4\">%d</value></Config>", number ? 1 : 0 );
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_FALSE(data.IsDirty());

  // set the value to the same value
  data.Set(L"value", number, false);

  // nothing should have changed.
  ASSERT_FALSE(data.IsDirty());
}

TEST_MEM(ConfigDataTest, ChangingTheValueTypeOnlyWillNotMakeItDirty)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config version=\"1\"><value type=\"11\">%d</value></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_FALSE(data.IsDirty());

  //  change the type, will not really change anything
  // internally it is still a long long int
  data.Set(L"value", (long int)number, false);

  // so it is not dirty
  ASSERT_FALSE(data.IsDirty());
}

TEST_MEM(ConfigDataTest, ChangingTheValueButNotTheTypeWillMakeItDirty)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config version=\"1\"><value type=\"11\">%d</value></Config>", number);
  ASSERT_TRUE(data.LoadXml(source));
  ASSERT_FALSE(data.IsDirty());

  // the type is not the same, but the type is.
  data.Set(L"value", IntRandomNumber<int>(), false);

  // so it is dirty
  ASSERT_TRUE(data.IsDirty());
}