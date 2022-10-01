#pragma once
#include "../string/string.h"
#include "Tags.h"

namespace myodd { namespace html {
  class HtmlData
  {
  protected:
    HtmlData(bool isHtmlTag, bool isEnd, bool isStartEnd, const std::wstring& text, Tag* tagData);

  public:
    HtmlData( bool isEnd, bool isStartEnd, Tag* tagData);
    HtmlData( const HtmlData& rhs );
    const HtmlData& operator=(const HtmlData& rhs) = delete;

    virtual ~HtmlData() = default;

    // helpers
    const int TextLength() const;
    const std::wstring& Text() const;

    const bool IsHtmlTag() const;
    const bool IsEnd() const;
    const bool IsStartEnd() const;
    Tag& TagData() const;

    const bool HasTagData() const;

    // apply the style
    void Push(HDC hdc, LOGFONT& logFont);

    // remove the style
    void Pop(HDC hdc, LOGFONT& logFont);

  protected:
    const std::wstring _text;
    const bool _isHtmlTag;
    const bool _isEnd;
    const bool _isStartEnd;
    Tag* _tag;
  };
}}
