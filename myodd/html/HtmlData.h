#pragma once
#include "../string/string.h"
#include "Tags.h"

namespace myodd { namespace html {
  class HtmlData
  {
  protected:
    HtmlData(bool isHtmlTag, bool isEnd, bool isStartEnd, const std::wstring& text, const std::wstring& attributes, Tag* tagData);

  public:
    HtmlData( bool isEnd, bool isStartEnd, const std::wstring& attributes, Tag* tagData);
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

  protected:
    const std::wstring _text;
    const std::wstring _attributes;
    const bool _isHtmlTag;
    const bool _isEnd;
    const bool _isStartEnd;
    Tag* _tag;
  };
}}
