#include "Attribute.h"
#include "AttributeValue.h"

namespace myodd{ namespace html{
Attribute::Attribute()
{
}

Attribute::~Attribute()
{
  Clear();
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

  // clear everything
  Clear();

  // copy variables.
  for (auto att : rhs._values)
  {
    Add(*att);
  }
}

void Attribute::Clear()
{
  for (auto att : _values)
  {
    delete att;
  }
  _values.clear();
}

void Attribute::Add(const AttributeValue& value)
{
  _values.push_back(AttributeValue::CreateFromSource(value));
}
}}