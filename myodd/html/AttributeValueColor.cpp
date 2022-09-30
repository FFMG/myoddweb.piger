#include "..\string\string.h"
#include "AttributeValueColor.h"

namespace myodd{ namespace html{
AttributeValueColor::AttributeValueColor(const std::wstring& color)
{
  _rgb = CreateColor(color);
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

AttributeValueColor::RGB AttributeValueColor::CreateColor(const std::wstring& color)
{
  auto lowerColor = myodd::strings::lower(color);
  if (lowerColor.length() == 0)
  {
    return {0,0,0};
  }

  if (color == L"red")
  {
    return { 255,0,0 };
  }
  if (color == L"blue")
  {
    return { 0, 0,255 };
  }
  if (color == L"green")
  {
    return { 0, 255,0 };
  }

  if (lowerColor[0] == L'#')
  {
    lowerColor = lowerColor.erase(0, 1);
    if (lowerColor.length() == 3)
    {
      auto r = lowerColor.substr(0, 1);
      auto g = lowerColor.substr(1, 1);
      auto b = lowerColor.substr(2, 1);
      lowerColor = r + r + g + g + b + b; //  make it size 6
    }
    if (lowerColor.length() == 6)
    {
      auto r = lowerColor.substr(0, 2);
      auto g = lowerColor.substr(2, 2);
      auto b = lowerColor.substr(4, 2);
      return { 
        HexToInt(r),
        HexToInt(g),
        HexToInt(b)
      };
    }
  }

  // not sure what color this is
  return { 0,0,0 };
}

unsigned short AttributeValueColor::HexToInt(const std::wstring& hex)
{
  unsigned short ur = 0;
  std::wstringstream rr;
  auto r = hex.substr(0, 2);
  rr << std::hex << r.c_str();
  rr >> ur;
  return ur;
}

void AttributeValueColor::Copy(const AttributeValueColor& rhs)
{
  if (this == &rhs)
  {
    return;
  }

  AttributeValue::Copy(rhs);

  // copy variables.
  _rgb = rhs._rgb;
}
}}