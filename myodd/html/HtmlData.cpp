#include "HtmlData.h"

namespace myodd { namespace html {
HtmlData::HtmlData(bool isHtmlTag, bool isEnd, bool isStartEnd, const std::wstring& text, const std::wstring& attributes, Tag* tagData) :
  _isHtmlTag(isHtmlTag),
  _isEnd(isEnd),
  _isStartEnd(isStartEnd),
  _text(text),
  _attributes(attributes),
  _tag(tagData)
{

}
HtmlData::HtmlData(bool isEnd, bool isStartEnd, const std::wstring& attributes, Tag* tagData) :
  HtmlData( true, isEnd, isStartEnd, L"", attributes, tagData)
{

}

HtmlData::HtmlData(const HtmlData& rhs) : 
  HtmlData(rhs._isHtmlTag, rhs._isEnd, rhs._isStartEnd, rhs._text, rhs._attributes, rhs._tag)
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

const bool HtmlData::IsStartEnd() const
{
  return _isStartEnd;
}

Tag& HtmlData::TagData() const
{
  return *_tag;
}

const bool HtmlData::HasTagData() const
{
  return _tag != nullptr;
}
}}