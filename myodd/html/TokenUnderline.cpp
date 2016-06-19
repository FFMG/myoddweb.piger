#include "TokenUnderline.h"
#include "../string/string.h"

TokenUnderline::TokenUnderline() : 
  Token()
{
}

TokenUnderline::~TokenUnderline()
{
}

// if this is the token we are looking for.
bool TokenUnderline::OnIsToken(const MYODD_CHAR* lpString, int nLen ) const
{
  if( nLen == 1 )
  {
    return (_tcsnicmp(lpString , _T("u"),	1 ) ==0);
  }
  return false;
}

// apply the style
void TokenUnderline::OnPush( LOGFONT& logFont )
{
  logFont.lfUnderline = 1;
}

// remove the style
void TokenUnderline::OnPop( LOGFONT& logFont )
{
  logFont.lfUnderline = 0;
}
