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

/**
 * \brief get the length of text
 * \return int the length
 */
const unsigned int DomObjectContent::TextLength() const
{
  return static_cast<unsigned int>(_text.length());
}

/**
 * \brief the text/content of this object
 * \return const std::wstring& the text
 */
const std::wstring& DomObjectContent::Text() const
{
  return _text;
}
}}