#pragma once
#include "../string/string.h"
#include "DomObject.h"

namespace myodd { namespace html {
  class SimpleHtmlData : public DomObject
  {
  public:
    SimpleHtmlData( const std::wstring& text );
    
    virtual ~SimpleHtmlData() = default;
  };
}}
