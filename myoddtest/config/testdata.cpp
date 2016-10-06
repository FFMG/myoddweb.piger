#include "config/data.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTest* 

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
  ASSERT_TRUE(data.FromXML(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Config><value type=\"8\">42</value></Config>"));
  ASSERT_EQ(42, data.Get( L"value"));
}
