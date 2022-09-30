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

  auto attr = parser->Parse( L"style='color:#f00;color:red;color:blue;'");
  ASSERT_EQ(1, attr.NumberOfAttributes());

  delete parser;
}