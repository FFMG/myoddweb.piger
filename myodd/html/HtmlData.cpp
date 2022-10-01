#include "HtmlData.h"

namespace myodd { namespace html {
HtmlData::HtmlData(bool isHtmlTag, bool isEnd, bool isStartEnd, const std::wstring& text, Tag* tagData) :
  _isHtmlTag(isHtmlTag),
  _isEnd(isEnd),
  _isStartEnd(isStartEnd),
  _text(text),
  _tag(tagData)
{

}
HtmlData::HtmlData(bool isEnd, bool isStartEnd, Tag* tagData) :
  HtmlData( true, isEnd, isStartEnd, L"", tagData)
{

}

HtmlData::HtmlData(const HtmlData& rhs) : 
  HtmlData(rhs._isHtmlTag, rhs._isEnd, rhs._isStartEnd, rhs._text, rhs._tag)
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

// apply the style
void HtmlData::Push(HDC hdc, LOGFONT& logFont)
{
  if (_tag != nullptr)
  {
    _tag->Push(hdc, logFont);
  }
}

// remove the style
void HtmlData::Pop(HDC hdc, LOGFONT& logFont)
{
  if (_tag != nullptr)
  {
    _tag->Pop(hdc, logFont);
  }
}

}}