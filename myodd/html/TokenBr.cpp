#include "TokenBr.h"
#include "../myodd.h"
#include "../string/string.h"

TokenBr::TokenBr() : 
  Token()
{
}

TokenBr::~TokenBr()
{
}

bool TokenBr::ToNextLine( bool bIsEnd ) const
{
  UNUSED_ALWAYS( bIsEnd );
  return true;
}

// if this is the token we are looking for.
bool TokenBr::OnIsToken(const MYODD_CHAR* lpString, int nLen ) const
{
  if( nLen == 2 )
  {
    return (_tcsnicmp(lpString , _T("br"),	1 ) ==0);
  }
  if( nLen == 3 )
  {
    return (_tcsnicmp(lpString , _T("br/"),	1 ) ==0);
  }
  return false;
}

void TokenBr::OnPush( LOGFONT& logFont )
{
  //  we do nothing
  UNUSED_ALWAYS( logFont );
}

void TokenBr::OnPop( LOGFONT& logFont )
{
  //  we do nothing
  UNUSED_ALWAYS( logFont );
}
