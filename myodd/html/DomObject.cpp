#include "DomObject.h"

namespace myodd { namespace html {
DomObject::DomObject(const std::wstring& text) :
  _text(text),
  _tag(nullptr)
{
}

DomObject::DomObject(const Tag& tagData)
{
  _tag = Tag::CreateFromSource(tagData);
}

DomObject::DomObject(const DomObject& rhs) :
  _text(rhs._text)
{
  _tag = rhs._tag == nullptr ? nullptr : Tag::CreateFromSource(*rhs._tag);
}

DomObject::~DomObject()
{
  delete _tag;
}

const int DomObject::TextLength() const
{
  return static_cast<int>(_text.length());
}

const std::wstring& DomObject::Text() const
{
  return _text;
}

Tag& DomObject::TagData() const
{
  return *_tag;
}

// apply the style
void DomObject::Push(HDC hdc, LOGFONT& logFont)
{
  if (_tag != nullptr)
  {
    _tag->Push(hdc, logFont);
  }
}

// remove the style
void DomObject::Pop(HDC hdc, LOGFONT& logFont)
{
  if (_tag != nullptr)
  {
    _tag->Pop(hdc, logFont);
  }
}

bool DomObject::IsHtmlTag() const
{
  return nullptr != _tag;
}

bool DomObject::IsClosing() const
{
  return _tag == nullptr ? false : _tag->Is(Tag::Closing);
}

bool DomObject::IsOpening() const
{
  return _tag == nullptr ? false : _tag->Is(Tag::Opening);
}
}}