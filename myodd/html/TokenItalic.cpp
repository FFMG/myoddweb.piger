#include "TokenItalic.h"
#include "../string/string.h"

TokenItalic::TokenItalic() : 
  Token()
{
}

TokenItalic::~TokenItalic()
{
}

// if this is the token we are looking for.
bool TokenItalic::OnIsToken(const wchar_t* lpString, __int64 nLen ) const
{
  if( nLen == 1 )
  {
    return (_tcsnicmp(lpString , L"i", nLen) ==0);
  }
  if( nLen == 2 )
  {
    return (_tcsnicmp(lpString , L"em", nLen) ==0);
  }
  return false;
}

// apply the style
void TokenItalic::OnPush( LOGFONT& logFont )
{
  logFont.lfItalic = 1;
}

// remove the style
void TokenItalic::OnPop( LOGFONT& logFont )
{
  logFont.lfItalic = 0;
}
