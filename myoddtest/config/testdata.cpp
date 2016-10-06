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
  data.Set(Uuid(), ::myodd::config::Data::type_string, Uuid(), true );
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