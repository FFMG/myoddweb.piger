#pragma once
#include "../string/string.h"
#include "HtmlData.h"

namespace myodd { namespace html {
  class SimpleHtmlData : public HtmlData
  {
  public:
    SimpleHtmlData( const std::wstring& text );
    
    virtual ~SimpleHtmlData() = default;
  };
}}
