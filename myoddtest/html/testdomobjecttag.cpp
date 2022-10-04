#include "html/TagsParser.h"
#include <gtest/gtest.h>

#include "html/DomObjectTag.h"
#include "html/TagBr.h"
#include "../testcommon.h"

TEST(DomObjectTag, AssigningAnNewTagDoesNotLeak)
{
  MEMORY_GUARD_START
  myodd::html::TagBr tag1(myodd::html::Attributes(), myodd::html::Tag::Opening );
  myodd::html::DomObjectTag dom( tag1 );

  // assign another value to it
  myodd::html::TagBr tag2(myodd::html::Attributes(), myodd::html::Tag::Opening);
  dom = myodd::html::DomObjectTag(tag2);

  MEMORY_GUARD_END
}