#include "TokenStrikeout.h"
#include "../string/string.h"

TokenStrikeout::TokenStrikeout() : 
Token()
{
}

TokenStrikeout::~TokenStrikeout()
{
}

// if this is the token we are looking for.
bool TokenStrikeout::OnIsToken(const wchar_t* lpString, __int64 nLen ) const
{
  if( nLen == 1 )
  {
    return (_tcsnicmp(lpString , _T("s"),	1 ) ==0);
  }
  if( nLen == 9 )
  {
    return (_tcsnicmp(lpString , _T("strikeout"),	1 ) ==0);
  }
  return false;
}

// apply the style
void TokenStrikeout::OnPush( LOGFONT& logFont )
{
  logFont.lfStrikeOut = 1;
}

// remove the style
void TokenStrikeout::OnPop( LOGFONT& logFont )
{
  logFont.lfStrikeOut = 0;
}
