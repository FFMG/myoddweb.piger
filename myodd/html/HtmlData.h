#pragma once
#include "../string/string.h"
#include "Tag.h"

namespace myodd { namespace html {
  class HtmlData
  {
  protected:
    HtmlData(const std::wstring& text, Tag* tagData);

  public:
    HtmlData( Tag* tagData);
    HtmlData( const HtmlData& rhs );
    const HtmlData& operator=(const HtmlData& rhs) = delete;

    virtual ~HtmlData();

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
