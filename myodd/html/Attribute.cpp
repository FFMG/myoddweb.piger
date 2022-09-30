#include "Attribute.h"

namespace myodd{ namespace html{
Attribute::Attribute()
{
}

Attribute::Attribute(const Attribute& rhs)
{
  Copy(rhs);
}

Attribute& Attribute::operator=(const Attribute& rhs)
{
  Copy(rhs);
  return *this;
}

void Attribute::Copy(const Attribute& rhs)
{
  if (this == &rhs)
  {
    return;
  }

  // copy variables.
}
}}