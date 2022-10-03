#include "DomObjectTag.h"

namespace myodd { namespace html {
DomObjectTag::DomObjectTag(const Tag& tagData)
{
  _tag = Tag::CreateFromSource(tagData);
}

DomObjectTag::DomObjectTag(const DomObjectTag& rhs)
{
  *this = rhs;
}

DomObjectTag& DomObjectTag::operator=(const DomObjectTag& rhs)
{
  if (this != &rhs)
  {
    delete _tag;
    DomObject::operator=(rhs);
    _tag = rhs._tag == nullptr ? nullptr : Tag::CreateFromSource(*rhs._tag);
  }
  return *this;
}

DomObjectTag::~DomObjectTag()
{
  delete _tag;
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