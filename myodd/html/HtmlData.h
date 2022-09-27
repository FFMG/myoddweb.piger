#pragma once
#include "../string/string.h"
#include "Tokens.h"

namespace myodd { namespace html {
  class HtmlData
  {
  protected:
    HtmlData(bool isHtmlTag, bool isEnd, const std::wstring& text, const std::wstring& attributes, Token* tokenData);

  public:
    HtmlData( bool isEnd, const std::wstring& attributes, Token* tokenData);
    HtmlData( const HtmlData& rhs );
    const HtmlData& operator=(const HtmlData& rhs) = delete;

    virtual ~HtmlData() = default;

    // helpers
    const int TextLength() const;
    const std::wstring& Text() const;

    const bool IsHtmlTag() const;
    const bool IsEnd() const;
    Token& TokenData() const;

    const bool HasTokenData() const;

  protected:
    const std::wstring _text;
    const std::wstring _attributes;
    const bool _isHtmlTag;
    const bool _isEnd;
    Token* _token;
  };
}}
