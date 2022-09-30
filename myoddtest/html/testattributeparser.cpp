#include "html/AttributesParser.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

TEST(BasicAttributesParser, NothingToParse)
{
  auto parser = new myodd::html::AttributesParser();

  parser->Parse(L"");

  delete parser;
}
