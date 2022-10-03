#include "DomObjectContent.h"

namespace myodd { namespace html {
DomObjectContent::DomObjectContent(const std::wstring& text) :
  _text(text)
{
}

DomObjectContent::DomObjectContent(const DomObjectContent& rhs)
{
  *this = rhs;
}

DomObjectContent& DomObjectContent::operator=(const DomObjectContent& rhs)
{
  if (this != &rhs)
  {
    DomObject::operator=(rhs);
    _text = rhs._text;
  }
  return *this;
}

const int DomObjectContent::TextLength() const
{
  return static_cast<int>(_text.length());
}

const std::wstring& DomObjectContent::Text() const
{
  return _text;
}

// apply the style
void DomObjectContent::Push(HDC hdc, LOGFONT& logFont)
{
}

// remove the style
void DomObjectContent::Pop(HDC hdc, LOGFONT& logFont)
{
}
}}