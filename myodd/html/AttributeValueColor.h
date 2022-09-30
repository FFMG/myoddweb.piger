#pragma once
#include "AttributeValue.h"
#include "../common/includes.h"
#include <windows.h>

namespace myodd{ namespace html{
class AttributeValueColor : public AttributeValue
{
public:
  AttributeValueColor();
  virtual ~AttributeValueColor() = default;

  AttributeValueColor(const AttributeValueColor&);
  AttributeValueColor& operator=(const AttributeValueColor&);
    
  // apply the style
  virtual void Push( LOGFONT& logFont );

  // remove the style
  virtual void Pop( LOGFONT& logFont );

protected:
  // copy the attribute values here.
  virtual void Copy(const AttributeValueColor& rhs);
};
}}