#include "html/TagsParser.h"
#include <gtest/gtest.h>

#include "html/DomObjectContent.h"
#include "html/DomObjectTag.h"
#include "../testcommon.h"

TEST(BasicTagsParser, NothingToParse)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"");
  ASSERT_EQ(0, dom.size()); //  nothing at all...

  delete parser;
}

TEST(BasicTagsParser, SpacesAreMaintained)
{
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
}

TEST(BasicTagsParser, BrTagIsNotAnEndTag)
{
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
}

TEST(BasicTagsParser, ClosedBrTagsAreLegal)
{
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
}

TEST(BasicTagsParser, NoHtmlTags)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"Hello");
  ASSERT_EQ(1, dom.size());

  auto isContent = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent != nullptr);
  ASSERT_TRUE(isContent->Text() == L"Hello");

  delete parser;
}

TEST(BasicTagsParser, SimpleSmallTag)
{
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
}

TEST(BasicTagsParser, NonExistentTag)
{
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
}

TEST(BasicTagsParser, TagIsNotClosed)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<foo1 and <foo2");
  ASSERT_EQ(1, dom.size()); //  we have 1 parts

  // none of them are html tags as we do not know them
  auto isContent0 = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent0 != nullptr);
  ASSERT_TRUE(isContent0->Text() == L"<foo1 and <foo2");

  delete parser;
}

TEST(BasicTagsParser, InvalidStartEndTagInsideAGoodNode)
{
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
}

TEST(BasicTagsParser, TagIsNotClosedThenWeHaveAGoodTag)
{
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
}

TEST(BasicTagsParser, TagIsNotClosedThenWeHaveAGoodTagWithBadTagsFater)
{
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
}

TEST(BasicTagsParser, JustOneCharacter)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L">");
  ASSERT_EQ(1, dom.size());

  // none of them are html tags as we do not know them
  auto isContent0 = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent0 != nullptr);
  ASSERT_TRUE(isContent0->Text() == L">");

  delete parser;
}

TEST(BasicTagsParser, NoOpenningTag)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"foo1>");
  ASSERT_EQ(1, dom.size());

  // none of them are html tags as we do not know them
  auto isContent0 = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent0 != nullptr);
  ASSERT_TRUE(isContent0->Text() == L"foo1>");

  delete parser;
}

TEST(BasicTagsParser, NoClosingTag)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<foo1");
  ASSERT_EQ(1, dom.size());

  // none of them are html tags as we do not know them
  auto isContent0 = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent0 != nullptr);
  ASSERT_TRUE(isContent0->Text() == L"<foo1");

  delete parser;
}

TEST(BasicTagsParser, TagIsNotOpen)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"foo>");
  ASSERT_EQ(1, dom.size()); //  we have 3 parts

  // none of them are html tags as we do not know them
  auto isContent0 = dynamic_cast<myodd::html::DomObjectContent*>(dom[0]);
  EXPECT_TRUE(isContent0 != nullptr);
  ASSERT_TRUE(isContent0->Text() == L"foo>");

  delete parser;
}

TEST(BasicTagsParser, TagCanBeClosedByItself)
{
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
}

TEST(BasicTagsParser, TagCanBeOpenByItself)
{
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
}