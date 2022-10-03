#include "DomObjectContent.h"

namespace myodd { namespace html {
DomObjectContent::DomObjectContent(const std::wstring& text) :
  _text(text)
{
}

DomObjectContent::DomObjectContent(const DomObjectContent& rhs) :
  _text(rhs._text)
{
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