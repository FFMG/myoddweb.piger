#pragma once
#include "AttributeValue.h"
#include "../common/includes.h"
#include <windows.h>

namespace myodd{ namespace html{
class AttributeValueColor : public AttributeValue
{
public:
  AttributeValueColor( const std::wstring& color );
  virtual ~AttributeValueColor() = default;

  AttributeValueColor(const AttributeValueColor&);
  AttributeValueColor& operator=(const AttributeValueColor&);
    
  // apply the style
  virtual void Push(HDC hdc, LOGFONT& logFont );

  // remove the style
  virtual void Pop(HDC hdc, LOGFONT& logFont );

protected:
  struct RGB
  {
    unsigned short r;
    unsigned short g;
    unsigned short b;
  };

  // copy the attribute values here.
  virtual void Copy(const AttributeValueColor& rhs);

  // the actual color
  RGB _rgb;

  static RGB CreateColor(const std::wstring& color);
  static unsigned short HexToInt(const std::wstring& hex );

  COLORREF _previousColor;
};
}}