#include "html/TagsParser.h"
#include <gtest/gtest.h>

#include "html/DomObjectContent.h"
#include "html/DomObjectTag.h"
#include "../testcommon.h"

TEST(BasicTagsParser, NothingToParse)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"");
  ASSERT_EQ(0, dom.size()); //  nothing at all...

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, SpacesAreMaintained)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"   <br>  ");
  ASSERT_EQ(3, dom.size());

  auto isContent0 = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent0!= nullptr);
  ASSERT_TRUE(isContent0->Text() == L"   ");

  auto isTag = dynamic_cast<myodd::html::DomObjectTag*>(dom[1]);
  EXPECT_TRUE(isTag != nullptr);

  auto isContent2 = dynamic_cast<myodd::html::DomObjectContent*>(dom[2]);
  EXPECT_TRUE(isContent2 != nullptr);
  ASSERT_TRUE(isContent2->Text() == L"  ");

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, BrTagIsNotAnEndTag)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  // this tag does not follow the open/close strict XML tag.
  // but it is legal html tag.
  auto dom = parser->Parse(L"<br>");
  ASSERT_EQ(1, dom.size());

  auto isTag = dynamic_cast<myodd::html::DomObjectTag*>(dom[0]);
  EXPECT_TRUE(isTag != nullptr);
  ASSERT_TRUE(isTag->IsClosing());
  ASSERT_TRUE(isTag->IsOpening());
  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, ClosedBrTagsAreLegal)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<br/><br />");
  ASSERT_EQ(2, dom.size());

  // they are both open/close tags

  auto isTag0 = dynamic_cast<myodd::html::DomObjectTag*>(dom[0]);
  EXPECT_TRUE(isTag0 != nullptr);
  ASSERT_TRUE(isTag0->IsClosing());
  ASSERT_TRUE(isTag0->IsOpening());

  auto isTag1 = dynamic_cast<myodd::html::DomObjectTag*>(dom[1]);
  EXPECT_TRUE(isTag1 != nullptr);
  ASSERT_TRUE(isTag1->IsClosing());
  ASSERT_TRUE(isTag1->IsOpening());

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, NoHtmlTags)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"Hello");
  ASSERT_EQ(1, dom.size());

  auto isContent = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent != nullptr);
  ASSERT_TRUE(isContent->Text() == L"Hello");

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, SimpleSmallTag)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<small>Hello</small>");
  ASSERT_EQ(3, dom.size()); //  we have 3 parts

  auto isTag0 = dynamic_cast<myodd::html::DomObjectTag*>(dom[0]);
  EXPECT_TRUE(isTag0 != nullptr);
  ASSERT_FALSE(isTag0->IsClosing());

  auto isContent = dynamic_cast<myodd::html::DomObjectContent*>(dom[1]);
  EXPECT_TRUE(isContent != nullptr);
  ASSERT_TRUE(isContent->Text() == L"Hello");

  auto isTag2 = dynamic_cast<myodd::html::DomObjectTag*>(dom[2]);
  EXPECT_TRUE(isTag2 != nullptr);
  ASSERT_TRUE(isTag2->IsClosing());

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, NonExistentTag)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<foo>Hello</foo>");
  ASSERT_EQ(3, dom.size()); //  we have 3 parts

  // none of them are html tags as we do not know them
  auto isContent0 = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent0 != nullptr);
  
  auto isContent1 = dynamic_cast<myodd::html::DomObjectContent*>(dom[1]);
  EXPECT_TRUE(isContent1 != nullptr);
  ASSERT_TRUE(isContent1->Text() == L"Hello");

  auto isContent2 = dynamic_cast<myodd::html::DomObjectContent*>(dom[2]);
  EXPECT_TRUE(isContent2 != nullptr);

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, TagIsNotClosed)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<foo1 and <foo2");
  ASSERT_EQ(1, dom.size()); //  we have 1 parts

  // none of them are html tags as we do not know them
  auto isContent0 = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent0 != nullptr);
  ASSERT_TRUE(isContent0->Text() == L"<foo1 and <foo2");

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, InvalidStartEndTagInsideAGoodNode)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<small><foo1 and <foo2</small>");
  ASSERT_EQ(3, dom.size());

  auto isTag0 = dynamic_cast<myodd::html::DomObjectTag*>(dom[0]);
  EXPECT_TRUE(isTag0 != nullptr);
  ASSERT_FALSE(isTag0->IsClosing());
  ASSERT_TRUE(isTag0->IsOpening());

  auto isContent1 = dynamic_cast<myodd::html::DomObjectContent*>(dom[1]);
  EXPECT_TRUE(isContent1 != nullptr);
  ASSERT_TRUE(isContent1->Text() == L"<foo1 and <foo2");

  auto isTag2 = dynamic_cast<myodd::html::DomObjectTag*>(dom[2]);
  EXPECT_TRUE(isTag2 != nullptr);
  ASSERT_TRUE(isTag2->IsClosing());
  ASSERT_FALSE(isTag2->IsOpening());

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, TagIsNotClosedThenWeHaveAGoodTag)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<foo1 and <foo2<small>Hello</small>");
  ASSERT_EQ(4, dom.size());

  auto isContent0 = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent0 != nullptr);
  ASSERT_TRUE(isContent0->Text() == L"<foo1 and <foo2");

  auto isTag1 = dynamic_cast<myodd::html::DomObjectTag*>(dom[1]);
  EXPECT_TRUE(isTag1 != nullptr);
  ASSERT_FALSE(isTag1->IsClosing());
  ASSERT_TRUE(isTag1->IsOpening());

  auto isContent2 = dynamic_cast<myodd::html::DomObjectContent*>(dom[2]);
  EXPECT_TRUE(isContent2 != nullptr);
  ASSERT_TRUE(isContent2->Text() == L"Hello");

  auto isTag3 = dynamic_cast<myodd::html::DomObjectTag*>(dom[3]);
  EXPECT_TRUE(isTag3 != nullptr);
  ASSERT_TRUE(isTag3->IsClosing());
  ASSERT_FALSE(isTag3->IsOpening());

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, TagIsNotClosedThenWeHaveAGoodTagWithBadTagsFater)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<foo1 and <foo2<small>Hello</small><More<Wrong");
  ASSERT_EQ(5, dom.size());

  auto isContent0 = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent0 != nullptr);
  ASSERT_TRUE(isContent0->Text() == L"<foo1 and <foo2");

  auto isTag1 = dynamic_cast<myodd::html::DomObjectTag*>(dom[1]);
  EXPECT_TRUE(isTag1 != nullptr);
  ASSERT_FALSE(isTag1->IsClosing());
  ASSERT_TRUE(isTag1->IsOpening());

  auto isContent2 = dynamic_cast<myodd::html::DomObjectContent*>(dom[2]);
  EXPECT_TRUE(isContent2 != nullptr);
  ASSERT_TRUE(isContent2->Text() == L"Hello");

  auto isTag3 = dynamic_cast<myodd::html::DomObjectTag*>(dom[3]);
  EXPECT_TRUE(isTag3 != nullptr);
  ASSERT_TRUE(isTag3->IsClosing());
  ASSERT_FALSE(isTag3->IsOpening());

  // thew rest is all wrong
  auto isContent4 = dynamic_cast<myodd::html::DomObjectContent*>(dom[4]);
  EXPECT_TRUE(isContent4 != nullptr);
  ASSERT_TRUE(isContent4->Text() == L"<More<Wrong");

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, JustOneCharacter)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L">");
  ASSERT_EQ(1, dom.size());

  // none of them are html tags as we do not know them
  auto isContent0 = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent0 != nullptr);
  ASSERT_TRUE(isContent0->Text() == L">");

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, NoOpenningTag)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"foo1>");
  ASSERT_EQ(1, dom.size());

  // none of them are html tags as we do not know them
  auto isContent0 = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent0 != nullptr);
  ASSERT_TRUE(isContent0->Text() == L"foo1>");

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, NoClosingTag)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<foo1");
  ASSERT_EQ(1, dom.size());

  // none of them are html tags as we do not know them
  auto isContent0 = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent0 != nullptr);
  ASSERT_TRUE(isContent0->Text() == L"<foo1");

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, TagIsNotOpen)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"foo>");
  ASSERT_EQ(1, dom.size()); //  we have 3 parts

  // none of them are html tags as we do not know them
  auto isContent0 = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent0 != nullptr);
  ASSERT_TRUE(isContent0->Text() == L"foo>");

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, TagCanBeClosedByItself)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"Foo</i>");
  ASSERT_EQ(2, dom.size());

  auto isContent0 = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent0 != nullptr);
  ASSERT_TRUE(isContent0->Text() == L"Foo");

  auto isTag1 = dynamic_cast<myodd::html::DomObjectTag*>(dom[1]);
  EXPECT_TRUE(isTag1 != nullptr);
  ASSERT_TRUE(isTag1->IsClosing());
  ASSERT_FALSE(isTag1->IsOpening());

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, TagCanBeOpenByItself)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"Foo<i>");
  ASSERT_EQ(2, dom.size());

  auto isContent0 = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent0 != nullptr);
  ASSERT_TRUE(isContent0->Text() == L"Foo");

  auto isTag1 = dynamic_cast<myodd::html::DomObjectTag*>(dom[1]);
  EXPECT_TRUE(isTag1 != nullptr);
  ASSERT_FALSE(isTag1->IsClosing());
  ASSERT_TRUE(isTag1->IsOpening());

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, CopyOfDomObjectDoesNotLeak)
{
  MEMORY_GUARD_START
  auto parser = new myodd::html::TagsParser();
  auto dom = parser->Parse(L"<i>Small Test<b>With Many Objects</b><strong>Here</strong></i>");

  // make a copy
  auto copyOfDom = myodd::html::DomObjects(dom);
  
  // the copy is the same 
  ASSERT_TRUE(dom.size() > 0);
  ASSERT_EQ(dom.size(), copyOfDom.size());

  delete parser;
  MEMORY_GUARD_END
}

TEST(BasicTagsParser, CopyOfDomObjectDoesNotLeakWhenASssignNewValue)
{
  MEMORY_GUARD_START
    auto parser = new myodd::html::TagsParser();
  auto dom = parser->Parse(L"<i>Small Test<b>With Many Objects</b><strong>Here</strong></i>");

  // make a copy
  auto copyOfDom = myodd::html::DomObjects(dom);

  // the copy is the same 
  ASSERT_TRUE(dom.size() > 0);
  ASSERT_EQ(dom.size(), copyOfDom.size());

  // give it a new value
  dom = parser->Parse(L"<i>Small</i>");

  // no longer the same
  ASSERT_TRUE(dom.size() > 0);
  ASSERT_FALSE(dom.size() == copyOfDom.size());

  delete parser;
  MEMORY_GUARD_END
}