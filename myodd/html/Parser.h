#pragma once

#include <vector>
#include "../string/string.h"
#include "Tokens.h"
#include "HtmlData.h"

namespace myodd { namespace html {
class Parser
{
public:
  Parser( HDC hdc );
  virtual ~Parser(void);

public:
  typedef std::vector<HtmlData*> HTML_CONTAINER;
  HTML_CONTAINER m_data;

  const HTML_CONTAINER& Parse(const wchar_t* lpString, int nCount );
  const HTML_CONTAINER& Tree() const{
    return m_data;
  };

public:
  SIZE Apply( HDC hdc, 
              const HtmlData* hd, 
              RECT& rect,
              const RECT& givenRect,
              const int maxLineHeight,
              const int paddingTop,
              const int paddingBottom,
              UINT uFormat );

  void DeInit( HDC hdc );

private:
  void Init( HDC hdc );
  void CalculateSmartDimensions(SIZE& size, HDC hDCScreen, const wchar_t* szText, int nLen);

  int mSaveDC;
  LOGFONT mLogFont;        //  the current font
  HFONT mFont;        //  the current font
  const LOGFONT& GetCurrentLogFont()const{
    return mLogFont;
  }

  //
  // the fonts
  void PushFont( HDC hdc, const LOGFONT& lf );
  void PopFont( HDC hdc, const LOGFONT& lf );
  void ApplyFont( HDC hdc, const LOGFONT& lf );

private:
  void Add(const wchar_t* begin, const wchar_t* end, bool isHtmlTag );
  void Clear();

  std::wstring EscapeText(const std::wstring& src) const;
  Token* FindToken(const std::wstring& text ) const;

  Tokens m_tokens;
};
}}