#include "HtmlData.h"

namespace myodd { namespace html {
HtmlData::HtmlData(bool isHtmlTag, bool isEnd, const std::wstring& text, const std::wstring& attributes, Token* tokenData) :
  _isHtmlTag(isHtmlTag),
  _isEnd(isEnd),
  _text(text),
  _attributes(attributes),
  _token(tokenData)
{

}
HtmlData::HtmlData(bool isEnd, const std::wstring& attributes, Token* tokenData) :
  HtmlData( true, isEnd, L"", attributes, tokenData)
{

}

HtmlData::HtmlData(const HtmlData& rhs) : 
  HtmlData(rhs._isHtmlTag, rhs._isEnd, rhs._text, rhs._attributes, rhs._token)
{
}

const int HtmlData::TextLength() const
{
  return static_cast<int>(_text.length());
}

const std::wstring& HtmlData::Text() const
{
  return _text;
}

const bool HtmlData::IsHtmlTag() const
{
  return _isHtmlTag;
}

const bool HtmlData::IsEnd() const
{
  return _isEnd;
}

Token& HtmlData::TokenData() const
{
  return *_token;
}

const bool HtmlData::HasTokenData() const
{
  return _token != nullptr;
}

}}