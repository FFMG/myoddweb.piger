#include "TokenBr.h"
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
bool TokenBr::OnIsToken(const wchar_t* lpString, __int64 nLen ) const
{
  if( nLen == 2 )
  {
    return (_tcsnicmp(lpString , L"br", nLen) ==0);
  }
  if( nLen == 3 )
  {
    return (_tcsnicmp(lpString , L"br/", nLen) ==0);
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
