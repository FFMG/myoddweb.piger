#include "html/AttributesParser.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

TEST(BasicAttributesParser, NothingToParse)
{
  auto parser = new myodd::html::AttributesParser();

  auto attr = parser->Parse(L"");
  ASSERT_EQ(0, attr.NumberOfAttributes());

  delete parser;
}

TEST(BasicAttributesParser, TryingToParseNullPtrDoesNotThrowAnError)
{
  auto parser = new myodd::html::AttributesParser();

  auto attr = parser->Parse(nullptr);
  ASSERT_EQ(0, attr.NumberOfAttributes());

  delete parser;
}

TEST(BasicAttributesParser, CheckThatTheNameIsValid)
{
  auto parser = new myodd::html::AttributesParser();

  auto attr = parser->Parse( L"style='color:red'");
  ASSERT_EQ(0, attr.NumberOfAttributes());

  delete parser;
}