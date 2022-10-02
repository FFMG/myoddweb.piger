#include "HtmlData.h"

namespace myodd { namespace html {
HtmlData::HtmlData(const std::wstring& text, Tag* tagData) :
  _text(text)
{
  _tag = tagData == nullptr ? nullptr :  Tags::CreateFromSource(*tagData);
}

HtmlData::HtmlData(Tag* tagData) :
  HtmlData( L"", tagData)
{

}

HtmlData::HtmlData(const HtmlData& rhs) : 
  HtmlData( rhs._text, rhs._tag)
{
}

HtmlData::~HtmlData()
{
  delete _tag;
}

const int HtmlData::TextLength() const
{
  return static_cast<int>(_text.length());
}

const std::wstring& HtmlData::Text() const
{
  return _text;
}

Tag& HtmlData::TagData() const
{
  return *_tag;
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

bool HtmlData::IsHtmlTag() const
{
  return nullptr != _tag;
}

bool HtmlData::IsClosing() const
{
  return _tag == nullptr ? false : _tag->Is(Tag::Closing);
}

bool HtmlData::IsOpening() const
{
  return _tag == nullptr ? false : _tag->Is(Tag::Opening);
}
}}