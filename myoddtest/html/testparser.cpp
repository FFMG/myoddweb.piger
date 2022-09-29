#include "html/TokensParser.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

TEST(BasicTokensParser, NothingToParse)
{
  auto parser = new myodd::html::TokensParser();

  auto dom = parser->Parse(L"");
  ASSERT_EQ(0, dom.size()); //  nothing at all...

  delete parser;
}

TEST(BasicTokensParser, SpacesAreMaintained)
{
  auto parser = new myodd::html::TokensParser();

  auto dom = parser->Parse(L"   <br>  ");
  ASSERT_EQ(3, dom.size());

  ASSERT_FALSE(dom[0]->IsHtmlTag());
  ASSERT_TRUE(dom[0]->Text() == L"   ");

  ASSERT_TRUE(dom[1]->IsHtmlTag());

  ASSERT_FALSE(dom[2]->IsHtmlTag());
  ASSERT_TRUE(dom[2]->Text() == L"  ");

  delete parser;
}

TEST(BasicTokensParser, BrTagIsNotAnEndTag)
{
  auto parser = new myodd::html::TokensParser();

  auto dom = parser->Parse(L"<br>");
  ASSERT_EQ(1, dom.size());

  ASSERT_TRUE(dom[0]->IsHtmlTag());
  ASSERT_FALSE(dom[0]->IsEnd());
  ASSERT_FALSE(dom[0]->IsStartEnd());   // We did not follow the xml strict mode
                                        // this tag was left 'open'
                                        // while it is valid it is not a start/end tag
  delete parser;
}

TEST(BasicTokensParser, ClosedBrTagsAreLegal)
{
  auto parser = new myodd::html::TokensParser();

  auto dom = parser->Parse(L"<br/><br />");
  ASSERT_EQ(2, dom.size());

  // NB: Although the tags are open/closed
  //     it is not an end tag as such ... it is just a tag
  //     that just happens to be closed right away.
  ASSERT_TRUE(dom[0]->IsHtmlTag());
  ASSERT_FALSE(dom[0]->IsEnd());
  ASSERT_TRUE(dom[0]->IsStartEnd());

  ASSERT_TRUE(dom[1]->IsHtmlTag());
  ASSERT_FALSE(dom[1]->IsEnd());
  ASSERT_TRUE(dom[1]->IsStartEnd());

  delete parser;
}

TEST(BasicTokensParser, NoHtmlTags)
{
  auto parser = new myodd::html::TokensParser();

  auto dom = parser->Parse(L"Hello");
  ASSERT_EQ(1, dom.size());

  ASSERT_FALSE(dom[0]->IsHtmlTag());
  ASSERT_TRUE(dom[0]->Text() == L"Hello");

  delete parser;
}

TEST(BasicTokensParser, SimpleSmallTag)
{
  auto parser = new myodd::html::TokensParser();

  auto dom = parser->Parse(L"<small>Hello</small>");
  ASSERT_EQ(3, dom.size()); //  we have 3 parts

  ASSERT_TRUE(dom[0]->IsHtmlTag());
  ASSERT_FALSE(dom[0]->IsEnd());

  ASSERT_FALSE(dom[1]->IsHtmlTag());
  ASSERT_TRUE(dom[1]->Text() == L"Hello");

  ASSERT_TRUE(dom[2]->IsHtmlTag());
  ASSERT_TRUE(dom[2]->IsEnd());

  delete parser;
}

TEST(BasicTokensParser, NonExistentTag)
{
  auto parser = new myodd::html::TokensParser();

  auto dom = parser->Parse(L"<foo>Hello</foo>");
  ASSERT_EQ(3, dom.size()); //  we have 3 parts

  // none of them are html tags as we do not know them
  ASSERT_FALSE(dom[0]->IsHtmlTag());
  
  ASSERT_FALSE(dom[1]->IsHtmlTag());
  ASSERT_TRUE(dom[1]->Text() == L"Hello");

  ASSERT_FALSE(dom[2]->IsHtmlTag());

  delete parser;
}

TEST(BasicTokensParser, TagIsNotClosed)
{
  auto parser = new myodd::html::TokensParser();

  auto dom = parser->Parse(L"<foo1 and <foo2");
  ASSERT_EQ(1, dom.size()); //  we have 1 parts

  // none of them are html tags as we do not know them
  ASSERT_FALSE(dom[0]->IsHtmlTag());

  delete parser;
}

TEST(BasicTokensParser, InvalidStartEndTagInsideAGoodNode)
{
  auto parser = new myodd::html::TokensParser();

  auto dom = parser->Parse(L"<small><foo1 and <foo2</small>");
  ASSERT_EQ(3, dom.size());

  ASSERT_TRUE(dom[0]->IsHtmlTag());   // open <small>
  ASSERT_FALSE(dom[0]->IsEnd());

  ASSERT_FALSE(dom[1]->IsHtmlTag());  // garbage text '<foo1 and <foo2'
  ASSERT_TRUE(dom[1]->Text() == L"<foo1 and <foo2");

  ASSERT_TRUE(dom[2]->IsHtmlTag());   // close </small>
  ASSERT_TRUE(dom[2]->IsEnd());   // close </small>

  delete parser;
}

TEST(BasicTokensParser, TagIsNotClosedThenWeHaveAGoodTag)
{
  auto parser = new myodd::html::TokensParser();

  auto dom = parser->Parse(L"<foo1 and <foo2<small>Hello</small>");
  ASSERT_EQ(4, dom.size());

  ASSERT_FALSE(dom[0]->IsHtmlTag());  // garbage text '<foo1 and <foo2'
  ASSERT_TRUE(dom[0]->Text() == L"<foo1 and <foo2");

  ASSERT_TRUE(dom[1]->IsHtmlTag());   // open <small>
  ASSERT_FALSE(dom[1]->IsEnd());

  ASSERT_FALSE(dom[2]->IsHtmlTag());  // text in tag 'Hello'
  ASSERT_TRUE(dom[2]->Text() == L"Hello");

  ASSERT_TRUE(dom[3]->IsHtmlTag());   // close </small>
  ASSERT_TRUE(dom[3]->IsEnd());   // close </small>

  delete parser;
}

TEST(BasicTokensParser, TagIsNotClosedThenWeHaveAGoodTagWithBadTagsFater)
{
  auto parser = new myodd::html::TokensParser();

  auto dom = parser->Parse(L"<foo1 and <foo2<small>Hello</small><More<Wrong");
  ASSERT_EQ(5, dom.size());

  ASSERT_FALSE(dom[0]->IsHtmlTag());  // garbage text '<foo1 and <foo2'
  ASSERT_TRUE(dom[0]->Text() == L"<foo1 and <foo2");

  ASSERT_TRUE(dom[1]->IsHtmlTag());   // open <small>
  ASSERT_FALSE(dom[1]->IsEnd());

  ASSERT_FALSE(dom[2]->IsHtmlTag());
  ASSERT_TRUE(dom[2]->Text() == L"Hello");

  ASSERT_TRUE(dom[3]->IsHtmlTag());   // close </small>
  ASSERT_TRUE(dom[3]->IsEnd());   // close </small>

  // thew rest is all wrong
  ASSERT_FALSE(dom[4]->IsHtmlTag());
  ASSERT_TRUE(dom[4]->Text() == L"<More<Wrong");

  delete parser;
}

TEST(BasicTokensParser, JustOneCharacter)
{
  auto parser = new myodd::html::TokensParser();

  auto dom = parser->Parse(L">");
  ASSERT_EQ(1, dom.size());

  // none of them are html tags as we do not know them
  ASSERT_FALSE(dom[0]->IsHtmlTag());  // garbage text
  ASSERT_TRUE(dom[0]->Text() == L">");

  delete parser;
}

TEST(BasicTokensParser, NoOpenningTag)
{
  auto parser = new myodd::html::TokensParser();

  auto dom = parser->Parse(L"foo1>");
  ASSERT_EQ(1, dom.size());

  // none of them are html tags as we do not know them
  ASSERT_FALSE(dom[0]->IsHtmlTag());  // garbage text
  ASSERT_TRUE(dom[0]->Text() == L"foo1>");

  delete parser;
}

TEST(BasicTokensParser, NoClosingTag)
{
  auto parser = new myodd::html::TokensParser();

  auto dom = parser->Parse(L"<foo1");
  ASSERT_EQ(1, dom.size());

  // none of them are html tags as we do not know them
  ASSERT_FALSE(dom[0]->IsHtmlTag());  // garbage text
  ASSERT_TRUE(dom[0]->Text() == L"<foo1");

  delete parser;
}

TEST(BasicTokensParser, TagIsNotOpen)
{
  auto parser = new myodd::html::TokensParser();

  auto dom = parser->Parse(L"foo>");
  ASSERT_EQ(1, dom.size()); //  we have 3 parts

  // none of them are html tags as we do not know them
  ASSERT_FALSE(dom[0]->IsHtmlTag());

  delete parser;
}