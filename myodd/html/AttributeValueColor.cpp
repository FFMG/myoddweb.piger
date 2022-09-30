#include "AttributeValueColor.h"

namespace myodd{ namespace html{
AttributeValueColor::AttributeValueColor()
{
}

AttributeValueColor::AttributeValueColor(const AttributeValueColor& rhs) :
  AttributeValue( rhs )
{
  Copy(rhs);
}

AttributeValueColor& AttributeValueColor::operator=(const AttributeValueColor& rhs)
{
  Copy(rhs);
  return *this;
}

// apply the style
void AttributeValueColor::Push(LOGFONT& logFont)
{

}

// remove the style
void AttributeValueColor::Pop(LOGFONT& logFont)
{

}


void AttributeValueColor::Copy(const AttributeValueColor& rhs)
{
  if (this == &rhs)
  {
    return;
  }

  AttributeValue::Copy(rhs);

  // copy variables.
}
}}