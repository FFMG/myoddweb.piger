#pragma once
#include "Attribute.h"
#include "AttributeValue.h"
#include "../common/includes.h"
#include <vector>

namespace myodd{ namespace html{
class AttributeStyle : public Attribute
{
public:
  AttributeStyle();
  virtual ~AttributeStyle();
  
  AttributeStyle(const AttributeStyle&);
  AttributeStyle& operator=(const AttributeStyle&);

  // apply the style
  virtual void Push( LOGFONT& logFont );

  // remove the style
  virtual void Pop( LOGFONT& logFont );

protected:
  // copy the attribute values here.
  virtual void Copy(const Attribute& rhs);

  void Clear();
};
}}