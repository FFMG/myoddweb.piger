#include "html/TagsParser.h"
#include <gtest/gtest.h>

#include "html/Attributes.h"
#include "html/AttributeStyle.h"
#include "html/AttributeValueColor.h"
#include "../testcommon.h"

TEST(TestAttributes, AssigningNewValuesDoNotLeak)
{
  MEMORY_GUARD_START
  myodd::html::Attributes attrs1;
  myodd::html::AttributeStyle attr1;

  attr1.Add(myodd::html::AttributeValueColor(L"#ff0000"));
  attr1.Add(myodd::html::AttributeValueColor(L"#00ff"));

  attrs1.Add(attr1);

  // assign another value to it
  myodd::html::AttributeStyle attr2;
  attr2.Add(myodd::html::AttributeValueColor(L"#ff0000"));

  attr1 = attr2;

  auto attrs2 = attrs1;

  MEMORY_GUARD_END
}