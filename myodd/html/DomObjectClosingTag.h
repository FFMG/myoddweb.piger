#pragma once
#include "DomObjectTag.h"

namespace myodd { namespace html {
  class DomObjectClosingTag : public DomObjectTag
  {
  public:
    DomObjectClosingTag( const Tag& tagData, Tag& tagParent );
    DomObjectClosingTag( const DomObjectClosingTag& rhs );
    DomObjectClosingTag& operator=(const DomObjectClosingTag& rhs) = delete;

    virtual ~DomObjectClosingTag();

    // apply the style
    void Push(HDC hdc, LOGFONT& logFont);

    // remove the style
    void Pop(HDC hdc, LOGFONT& logFont);

  private:
    //  
    Tag& _tagParent;
  };
}}
