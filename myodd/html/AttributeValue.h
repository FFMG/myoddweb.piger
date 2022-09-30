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
  virtual void Push(HDC hdc, LOGFONT& logFont ) = 0;

  // remove the style
  virtual void Pop(HDC hdc, LOGFONT& logFont ) = 0;

  /**
   * \brief create an attribute from another attribute.
   *        we cast the given attribute so we create the correct derived class.
   * \param const AttributeValue& the value we want to copy.
   * \return AttributeValue* the value with the correct derived class.
   */
  static AttributeValue* CreateFromSource(const AttributeValue& src);

  /**
   * \brief create an attribute from another attribute.
   *        we cast the given attribute so we create the correct derived class.
   * \param const std::wstring& the property:value that we are trying to parse.
   * \return AttributeValue* the value with the correct derived class.
   */
  static AttributeValue* CreateFromString(const std::wstring& propertyAndValue );

protected:
  // copy the attribute values here.
  virtual void Copy(const AttributeValue& rhs);
};
}}