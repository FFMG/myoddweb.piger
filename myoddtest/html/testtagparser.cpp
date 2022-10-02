#include "html/TagsParser.h"
#include <gtest/gtest.h>

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

  ASSERT_FALSE(dom[0]->IsHtmlTag());
  ASSERT_TRUE(dom[0]->Text() == L"   ");

  ASSERT_TRUE(dom[1]->IsHtmlTag());

  ASSERT_FALSE(dom[2]->IsHtmlTag());
  ASSERT_TRUE(dom[2]->Text() == L"  ");

  delete parser;
}

TEST(BasicTagsParser, BrTagIsNotAnEndTag)
{
  auto parser = new myodd::html::TagsParser();

  // this tag does not follow the open/close strict XML tag.
  // but it is legal html tag.
  auto dom = parser->Parse(L"<br>");
  ASSERT_EQ(1, dom.size());

  ASSERT_TRUE(dom[0]->IsHtmlTag());
  ASSERT_TRUE(dom[0]->IsClosing());
  ASSERT_TRUE(dom[0]->IsOpening());
  delete parser;
}

TEST(BasicTagsParser, ClosedBrTagsAreLegal)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<br/><br />");
  ASSERT_EQ(2, dom.size());

  // they are both open/close tags

  ASSERT_TRUE(dom[0]->IsHtmlTag());
  ASSERT_TRUE(dom[0]->IsClosing());
  ASSERT_TRUE(dom[0]->IsOpening());

  ASSERT_TRUE(dom[1]->IsHtmlTag());
  ASSERT_TRUE(dom[1]->IsClosing());
  ASSERT_TRUE(dom[1]->IsOpening());

  delete parser;
}

TEST(BasicTagsParser, NoHtmlTags)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"Hello");
  ASSERT_EQ(1, dom.size());

  ASSERT_FALSE(dom[0]->IsHtmlTag());
  ASSERT_TRUE(dom[0]->Text() == L"Hello");

  delete parser;
}

TEST(BasicTagsParser, SimpleSmallTag)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<small>Hello</small>");
  ASSERT_EQ(3, dom.size()); //  we have 3 parts

  ASSERT_TRUE(dom[0]->IsHtmlTag());
  ASSERT_FALSE(dom[0]->IsClosing());

  ASSERT_FALSE(dom[1]->IsHtmlTag());
  ASSERT_TRUE(dom[1]->Text() == L"Hello");

  ASSERT_TRUE(dom[2]->IsHtmlTag());
  ASSERT_TRUE(dom[2]->IsClosing());

  delete parser;
}

TEST(BasicTagsParser, NonExistentTag)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<foo>Hello</foo>");
  ASSERT_EQ(3, dom.size()); //  we have 3 parts

  // none of them are html tags as we do not know them
  ASSERT_FALSE(dom[0]->IsHtmlTag());
  
  ASSERT_FALSE(dom[1]->IsHtmlTag());
  ASSERT_TRUE(dom[1]->Text() == L"Hello");

  ASSERT_FALSE(dom[2]->IsHtmlTag());

  delete parser;
}

TEST(BasicTagsParser, TagIsNotClosed)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<foo1 and <foo2");
  ASSERT_EQ(1, dom.size()); //  we have 1 parts

  // none of them are html tags as we do not know them
  ASSERT_FALSE(dom[0]->IsHtmlTag());

  delete parser;
}

TEST(BasicTagsParser, InvalidStartEndTagInsideAGoodNode)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<small><foo1 and <foo2</small>");
  ASSERT_EQ(3, dom.size());

  ASSERT_TRUE(dom[0]->IsHtmlTag());   // open <small>
  ASSERT_FALSE(dom[0]->IsClosing());

  ASSERT_FALSE(dom[1]->IsHtmlTag());  // garbage text '<foo1 and <foo2'
  ASSERT_TRUE(dom[1]->Text() == L"<foo1 and <foo2");

  ASSERT_TRUE(dom[2]->IsHtmlTag());   // close </small>
  ASSERT_TRUE(dom[2]->IsClosing());   // close </small>

  delete parser;
}

TEST(BasicTagsParser, TagIsNotClosedThenWeHaveAGoodTag)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<foo1 and <foo2<small>Hello</small>");
  ASSERT_EQ(4, dom.size());

  ASSERT_FALSE(dom[0]->IsHtmlTag());  // garbage text '<foo1 and <foo2'
  ASSERT_TRUE(dom[0]->Text() == L"<foo1 and <foo2");

  ASSERT_TRUE(dom[1]->IsHtmlTag());   // open <small>
  ASSERT_FALSE(dom[1]->IsClosing());

  ASSERT_FALSE(dom[2]->IsHtmlTag());  // text in tag 'Hello'
  ASSERT_TRUE(dom[2]->Text() == L"Hello");

  ASSERT_TRUE(dom[3]->IsHtmlTag());   // close </small>
  ASSERT_TRUE(dom[3]->IsClosing());   // close </small>

  delete parser;
}

TEST(BasicTagsParser, TagIsNotClosedThenWeHaveAGoodTagWithBadTagsFater)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<foo1 and <foo2<small>Hello</small><More<Wrong");
  ASSERT_EQ(5, dom.size());

  ASSERT_FALSE(dom[0]->IsHtmlTag());  // garbage text '<foo1 and <foo2'
  ASSERT_TRUE(dom[0]->Text() == L"<foo1 and <foo2");

  ASSERT_TRUE(dom[1]->IsHtmlTag());   // open <small>
  ASSERT_FALSE(dom[1]->IsClosing());

  ASSERT_FALSE(dom[2]->IsHtmlTag());
  ASSERT_TRUE(dom[2]->Text() == L"Hello");

  ASSERT_TRUE(dom[3]->IsHtmlTag());   // close </small>
  ASSERT_TRUE(dom[3]->IsClosing());   // close </small>

  // thew rest is all wrong
  ASSERT_FALSE(dom[4]->IsHtmlTag());
  ASSERT_TRUE(dom[4]->Text() == L"<More<Wrong");

  delete parser;
}

TEST(BasicTagsParser, JustOneCharacter)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L">");
  ASSERT_EQ(1, dom.size());

  // none of them are html tags as we do not know them
  ASSERT_FALSE(dom[0]->IsHtmlTag());  // garbage text
  ASSERT_TRUE(dom[0]->Text() == L">");

  delete parser;
}

TEST(BasicTagsParser, NoOpenningTag)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"foo1>");
  ASSERT_EQ(1, dom.size());

  // none of them are html tags as we do not know them
  ASSERT_FALSE(dom[0]->IsHtmlTag());  // garbage text
  ASSERT_TRUE(dom[0]->Text() == L"foo1>");

  delete parser;
}

TEST(BasicTagsParser, NoClosingTag)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"<foo1");
  ASSERT_EQ(1, dom.size());

  // none of them are html tags as we do not know them
  ASSERT_FALSE(dom[0]->IsHtmlTag());  // garbage text
  ASSERT_TRUE(dom[0]->Text() == L"<foo1");

  delete parser;
}

TEST(BasicTagsParser, TagIsNotOpen)
{
  auto parser = new myodd::html::TagsParser();

  auto dom = parser->Parse(L"foo>");
  ASSERT_EQ(1, dom.size()); //  we have 3 parts

  // none of them are html tags as we do not know them
  ASSERT_FALSE(dom[0]->IsHtmlTag());

  delete parser;
}