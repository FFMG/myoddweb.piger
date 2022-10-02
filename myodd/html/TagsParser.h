#pragma once

#include <vector>
#include "../string/string.h"
#include "DomObject.h"

namespace myodd { namespace html {

typedef std::vector<DomObject*> DomObjects;

class TagsParser
{
public:
  TagsParser();
  virtual ~TagsParser(void);

public:
  const DomObjects& Parse( const std::wstring& text );
  const DomObjects& Parse(const wchar_t* lpString);

private:
  
  DomObjects m_data;
  const DomObjects& Tree() const {
    return m_data;
  };

public:
  SIZE Apply( HDC hdc, 
              DomObject* hd, 
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

  const LOGFONT& GetCurrentLogFont() const;

  //
  // the fonts
  void PushFont( HDC hdc, const LOGFONT& lf );
  void PopFont( HDC hdc, const LOGFONT& lf );
  void ApplyFont( HDC hdc, const LOGFONT& lf );

private:
  void AddNonHtmlTag( const std::wstring& src );
  void AddNonHtmlTag(const wchar_t* begin, const wchar_t* end );
  void AddHtmlTag(const wchar_t* begin, const wchar_t* end);
  void Clear();

  std::wstring EscapeText(const std::wstring& src) const;

  const wchar_t* FindTag(const wchar_t* body, const wchar_t tag) const;
  const wchar_t* FindTagExcluding(const wchar_t* body, const wchar_t tag, const wchar_t exclude ) const;

  int _saveDC;
  LOGFONT _logFont;        //  the current font
  HFONT _font;        //  the current font
};
}}