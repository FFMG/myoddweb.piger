#pragma once
#include "../string/string.h"
#include "Tag.h"
#include "DomObject.h"

namespace myodd { namespace html {
  class DomObjectTag : public DomObject
  {
  public:
    DomObjectTag( const Tag& tagData);
    DomObjectTag( const DomObjectTag& rhs );
    DomObjectTag& operator=(const DomObjectTag& rhs);

    virtual ~DomObjectTag();

    bool IsClosing() const;
    bool IsOpening() const;
    Tag& TagData() const;

    // apply the style
    void Push(HDC hdc, LOGFONT& logFont);

    // remove the style
    void Pop(HDC hdc, LOGFONT& logFont);

  protected:
    /**
     * \brief the tag we are working with.
     */
    Tag* _tag;
  };
}}
