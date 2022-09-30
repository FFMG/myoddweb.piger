#include "..\string\string.h"
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

AttributeValue* AttributeValue::CreateFromString(const std::wstring& propertyAndValue)
{
  std::wstring property;
  std::wstring value;

  bool foundColon = false;
  for (auto current : propertyAndValue)
  {
    if (current == L':')
    {
      foundColon = true;
      continue;
    }
    if (foundColon)
    {
      value += current;
      continue;
    }

    property += current;    
  }

  const auto lowerProperty = myodd::strings::lower(property);
  const auto givenProperty = lowerProperty.c_str();
  const auto givenPropertyLength = lowerProperty.length();
  switch (givenPropertyLength)
  {
  case 5:
    if (_tcsnicmp(givenProperty, L"color", givenPropertyLength) == 0)
    {
      return new AttributeValueColor( value );
    }
  }
  return nullptr;
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