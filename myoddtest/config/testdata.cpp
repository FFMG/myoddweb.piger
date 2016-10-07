#include "config/data.h"
#include "string\string.h"
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
  data.Set(XmlElementName(), ::myodd::config::Data::type_string, Uuid(), true );
}

TEST_MEM(ConfigDataTest, LoadFromAnXML)
{
  ::myodd::config::Data data;
  ASSERT_TRUE( data.FromXML(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><value type=\"8\">42</value></Config>") );
}

TEST_MEM(ConfigDataTest, LoadFromAnXMLAndGetTheValueSingleDepth)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format( L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><value type=\"8\">%d</value></Config>", number );
  ASSERT_TRUE(data.FromXML(source));
  ASSERT_EQ(number, data.Get( L"value"));
}

TEST_MEM(ConfigDataTest, LoadFromAnXMLAndGetTheValueDoubleDepth)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.FromXML(source));
  ASSERT_EQ(number, data.Get(L"parent\\value"));
}

TEST_MEM(ConfigDataTest, LoadFromAnXMLAndGetTheValueDoubleDepthDifferentGetCases)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.FromXML(source));
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
  ASSERT_TRUE(data.FromXML(source));
  ASSERT_EQ(number, data.Get(L"parent\\value"));
  ASSERT_EQ(number, data.Get(L"parent\\VALUE"));
  ASSERT_EQ(number, data.Get(L"Parent\\Value"));
}

TEST_MEM(ConfigDataTest, GetAValueThatDoesNotExist)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.FromXML(source));
  ASSERT_EQ(number, data.Get(L"parent\\value"));
  EXPECT_THROW(data.Get(L"something\\else"), std::exception);
}

TEST_MEM(ConfigDataTest, GetAValueWithSpaces)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.FromXML(source));
  ASSERT_EQ(number, data.Get(L"     parent\\value    "));
}

TEST_MEM(ConfigDataTest, GetAValueWithExtrabackslashes)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.FromXML(source));
  ASSERT_EQ(number, data.Get(L"\\parent\\value\\"));
}

TEST_MEM(ConfigDataTest, GetAValueWithMultiplebackslashes)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.FromXML(source));
  ASSERT_EQ(number, data.Get(L"\\\\parent\\\\\\value\\\\\\"));
}

TEST_MEM(ConfigDataTest, NumersAndMinusAreValidElementNames)
{
  //  invalid XML, (just number <123-value>sss</123-value> will not be accepted at load time)
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><Value-123 type=\"8\">%d</Value-123></parent></Config>", number);
  ASSERT_TRUE(data.FromXML(source));
  ASSERT_EQ(number, data.Get(L"parent\\Value-123"));
}

TEST_MEM(ConfigDataTest, NumersAreValidElementNames)
{
  //  invalid XML, (just number <123>sss</123> will not be accepted at load time)
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><x123 type=\"8\">%d</x123></parent></Config>", number);
  ASSERT_TRUE(data.FromXML(source));
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
  ASSERT_TRUE(data.FromXML(source));
  ASSERT_TRUE( data.Contains(L"parent\\value"));
}

TEST_MEM(ConfigDataTest, CheckIfContainsTheValueVariousCase)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.FromXML(source));
  ASSERT_TRUE(data.Contains(L"Parent\\Value"));
  ASSERT_TRUE(data.Contains(L"Parent\\VALUE"));
  ASSERT_TRUE(data.Contains(L"PAREnt\\Value"));
}

TEST_MEM(ConfigDataTest, GetDefaultValueForNonExistent)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.FromXML(source));

  auto defaultNumber = IntRandomNumber<int>();
  ASSERT_EQ(defaultNumber, data.Get(L"some\\other\\value", defaultNumber));
}

TEST_MEM(ConfigDataTest, GetDefaultValueForNonExistentButAlmostValid)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.FromXML(source));

  auto defaultNumber = IntRandomNumber<int>();
  // almost the same as parent\\value
  ASSERT_EQ(defaultNumber, data.Get(L"parent\\value1", defaultNumber));
}

TEST_MEM(ConfigDataTest, GetDefaultValueButValueExists)
{
  ::myodd::config::Data data;
  auto number = IntRandomNumber<int>();
  auto source = ::myodd::strings::Format(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><parent><value type=\"8\">%d</value></parent></Config>", number);
  ASSERT_TRUE(data.FromXML(source));

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
  EXPECT_THROW(data.Set(L"1234", ::myodd::config::Data::type_int, 1234, false), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, YouCannotSetANameWithANumberEvenWithMinus)
{
  // those are not valid xml element names
  ::myodd::config::Data data;
  EXPECT_THROW(data.Set(L"1234-5678", ::myodd::config::Data::type_int, 1234, false), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, YouCannotSetANameWithANumberOnlyEvenInThePath)
{
  // those are not valid xml element names
  ::myodd::config::Data data;
  EXPECT_THROW(data.Set(L"valid\\path\\1234", ::myodd::config::Data::type_int, 1234, false), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, YouCannotSetAnEllementThatStartsWithAMinus)
{
  // those are not valid xml element names
  ::myodd::config::Data data;
  data.Set(L"valid\\path\\good-name", ::myodd::config::Data::type_int, 1234, false);
  EXPECT_THROW(data.Set(L"valid\\path\\-bad", ::myodd::config::Data::type_int, 1234, false), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, YouCannotSetANameWithANumberWithMinusOnlyEvenInThePath)
{
  // those are not valid xml element names
  ::myodd::config::Data data;
  EXPECT_THROW(data.Set(L"valid\\path\\1234-5678", ::myodd::config::Data::type_int, 1234, false), std::invalid_argument);
}

TEST_MEM(ConfigDataTest, YouCannotSetANameWithANumberOnlyAtTheBeginingEvenInThePath)
{
  // those are not valid xml element names
  ::myodd::config::Data data;
  EXPECT_THROW(data.Set(L"1234\\valid\\path", ::myodd::config::Data::type_int, 1234, false), std::invalid_argument);
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
  data.Set(name, ::myodd::config::Data::type_int, number, false);
  ASSERT_FALSE(data.IsTemp(name));
  
  // make it temp
  data.Set(name, ::myodd::config::Data::type_int, number, true);
  ASSERT_TRUE(data.IsTemp(name));

  // make it permanent
  data.Set(name, ::myodd::config::Data::type_int, number, false);
  ASSERT_FALSE(data.IsTemp(name));
}
