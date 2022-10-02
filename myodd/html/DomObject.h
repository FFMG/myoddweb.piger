#pragma once
#include "../string/string.h"
#include "Tag.h"

namespace myodd { namespace html {
  class DomObject
  {
  protected:
    DomObject(const std::wstring& text);

  public:
    DomObject( const Tag& tagData);
    DomObject( const DomObject& rhs );
    const DomObject& operator=(const DomObject& rhs) = delete;

    virtual ~DomObject();

    // helpers
    const int TextLength() const;
    const std::wstring& Text() const;

    bool IsHtmlTag() const;
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
