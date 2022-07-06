#pragma once

#include <vector>
#include "../string/string.h"

#include "Tokens.h"

class Parser
{
public:
  Parser( HDC hdc );
  virtual ~Parser(void);

public:
  struct HTMLDATA
  {
    HTMLDATA() : mIsHtmlTag(false), mToken(0), mIsEnd(false){};
    std::wstring text;
    std::wstring attributes;
    bool mIsHtmlTag;
    bool mIsEnd;
    Token* mToken;
  };
  typedef std::vector<HTMLDATA*> HTML_CONTAINER;
  HTML_CONTAINER m_data;

  const HTML_CONTAINER& Parse(const MYODD_CHAR* lpString, int nCount );
  const HTML_CONTAINER& Tree() const{
    return m_data;
  };

public:
  SIZE Apply( HDC hdc, 
              const Parser::HTMLDATA* hd, 
              RECT& rect,
              const RECT& givenRect,
              UINT uFormat );

  void DeInit( HDC hdc );

private:
  void Init( HDC hdc );
  
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
  void Add(const MYODD_CHAR* begin, LPCTSTR end, bool isHtmlTag );
  void Clear();
};
