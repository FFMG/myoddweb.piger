#include "TokenBold.h"

TokenBold::TokenBold() : 
  Token()
{
}

TokenBold::~TokenBold()
{
}

// if this is the token we are looking for.
bool TokenBold::OnIsToken( LPCTSTR lpString, int nLen ) const
{
  if( nLen == 1 )
  {
    return (_tcsnicmp(lpString , _T("b"),	1 ) ==0);
  }
  if( nLen == 6 )
  {
    return (_tcsnicmp(lpString , _T("strong"),	1 ) ==0);
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
