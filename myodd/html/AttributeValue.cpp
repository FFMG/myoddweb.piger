#include "AttributeValue.h"
#include "AttributeValueColor.h"

namespace myodd{ namespace html{
AttributeValue::AttributeValue()
{
}

AttributeValue::AttributeValue(const AttributeValue& rhs)
{
  Copy(rhs);
}

AttributeValue& AttributeValue::operator=(const AttributeValue& rhs)
{
  Copy(rhs);
  return *this;
}

void AttributeValue::Copy(const AttributeValue& rhs)
{
  if (this == &rhs)
  {
    return;
  }

  // copy variables.
}

AttributeValue* AttributeValue::CreateFromSource(const AttributeValue& src)
{
  auto valueColorPtr = dynamic_cast<const AttributeValueColor*>(&src);
  if (nullptr != valueColorPtr)
  {
    return new AttributeValueColor(*valueColorPtr);
  }

  assert(0);  //  unknown attribute
  return nullptr;
}
}}