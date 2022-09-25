#include "TokenBold.h"
#include "../string/string.h"

TokenBold::TokenBold() : 
  Token()
{
}

TokenBold::~TokenBold()
{
}

// if this is the token we are looking for.
bool TokenBold::OnIsToken(const wchar_t* lpString, __int64 nLen ) const
{
  if( nLen == 1 )
  {
    return (_tcsnicmp(lpString , L"b", nLen) ==0);
  }
  if( nLen == 6 )
  {
    return (_tcsnicmp(lpString , L"strong", nLen) ==0);
  }
  return false;
}

// apply the style
void TokenBold::OnPush( LOGFONT& logFont )
{
  logFont.lfWeight = FW_BOLD;
}

// remove the style
void TokenBold::OnPop( LOGFONT& logFont )
{
  logFont.lfWeight = FW_NORMAL;
}
