#pragma once
#include "../string/string.h"
#include "Tag.h"
#include "DomObject.h"

namespace myodd { namespace html {
  class DomObjectTag : public DomObject
  {
  protected:
    DomObjectTag(const std::wstring& text);

  public:
    DomObjectTag( const Tag& tagData);
    DomObjectTag( const DomObjectTag& rhs );
    const DomObjectTag& operator=(const DomObjectTag& rhs) = delete;

    virtual ~DomObjectTag();

    // helpers
    const int TextLength() const;
    const std::wstring& Text() const;

    bool IsClosing() const;
    bool IsOpening() const;
    Tag& TagData() const;

    // apply the style
    void Push(HDC hdc, LOGFONT& logFont);

    // remove the style
    void Pop(HDC hdc, LOGFONT& logFont);

  protected:
    const std::wstring _text;
    Tag* _tag;
  };
}}
