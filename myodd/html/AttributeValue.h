#pragma once

#include "../common/includes.h"
#include <windows.h>

namespace myodd{ namespace html{
class AttributeValue
{
public:
  AttributeValue();
  virtual ~AttributeValue() = default;

  AttributeValue(const AttributeValue&);
  AttributeValue& operator=(const AttributeValue&);
    
  // apply the style
  virtual void Push( LOGFONT& logFont ) = 0;

  // remove the style
  virtual void Pop( LOGFONT& logFont ) = 0;

  /**
   * \brief create an attribute from another attribute.
   *        we cast the given attribute so we create the correct derived class.
   * \return AttributeValue* the value with the correct derived class.
   */
  static AttributeValue* CreateFromSource(const AttributeValue& src);

protected:
  // copy the attribute values here.
  virtual void Copy(const AttributeValue& rhs);
};
}}