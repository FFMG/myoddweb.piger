#pragma once
#include "Attributes.h"
#include <string>

namespace myodd { namespace html {
class AttributesParser
{
public:
  AttributesParser();
  virtual ~AttributesParser(void);

public:
  Attributes Parse( const std::wstring& text ) const;
  Attributes Parse( const wchar_t* lpString) const;

private:
};
}}