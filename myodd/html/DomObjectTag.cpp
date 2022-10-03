#include "DomObjectTag.h"

namespace myodd { namespace html {
DomObjectTag::DomObjectTag(const std::wstring& text) : DomObject(),
  _text(text),
  _tag(nullptr)
{
}

DomObjectTag::DomObjectTag(const Tag& tagData)
{
  _tag = Tag::CreateFromSource(tagData);
}

DomObjectTag::DomObjectTag(const DomObjectTag& rhs) :
  _text(rhs._text)
{
  _tag = rhs._tag == nullptr ? nullptr : Tag::CreateFromSource(*rhs._tag);
}

DomObjectTag::~DomObjectTag()
{
  delete _tag;
}

const int DomObjectTag::TextLength() const
{
  return static_cast<int>(_text.length());
}

const std::wstring& DomObjectTag::Text() const
{
  return _text;
}

Tag& DomObjectTag::TagData() const
{
  return *_tag;
}

// apply the style
void DomObjectTag::Push(HDC hdc, LOGFONT& logFont)
{
  if (_tag != nullptr)
  {
    _tag->Push(hdc, logFont);
  }
}

// remove the style
void DomObjectTag::Pop(HDC hdc, LOGFONT& logFont)
{
  if (_tag != nullptr)
  {
    _tag->Pop(hdc, logFont);
  }
}

bool DomObjectTag::IsClosing() const
{
  return _tag == nullptr ? false : _tag->Is(Tag::Closing);
}

bool DomObjectTag::IsOpening() const
{
  return _tag == nullptr ? false : _tag->Is(Tag::Opening);
}
}}