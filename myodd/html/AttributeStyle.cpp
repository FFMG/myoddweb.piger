#include "AttributeStyle.h"
#include "AttributeValueColor.h"

namespace myodd{ namespace html{
AttributeStyle::AttributeStyle()
{
}

AttributeStyle::AttributeStyle(const AttributeStyle& src) : 
  Attribute( src )
{
  Copy(src);
}

AttributeStyle::~AttributeStyle()
{
  Clear();
}

AttributeStyle& AttributeStyle::operator=(const AttributeStyle& src)
{
  Copy(src);
  return *this;
}

void AttributeStyle::Clear()
{
}

// apply the styles
void AttributeStyle::Push(HDC hdc, LOGFONT& logFont)
{
}

// remove the style
void AttributeStyle::Pop(HDC hdc, LOGFONT& logFont)
{
}

void AttributeStyle::Copy(const Attribute& rhs)
{
  if (this == &rhs)
  {
    return;
  }

  // clear our values
  Clear();

  Attribute::Copy(rhs);
}
}}