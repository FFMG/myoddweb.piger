#include "TokenSmall.h"
#include "../string/string.h"

TokenSmall::TokenSmall() :
  Token(),
  _lfHeight( 0)
{
}

TokenSmall::~TokenSmall()
{
}

// if this is the token we are looking for.
bool TokenSmall::OnIsToken(const wchar_t* lpString, __int64 nLen ) const
{
  if( nLen == 5 )
  {
    return (_tcsnicmp(lpString , L"small", nLen) ==0);
  }
  return false;
}

// apply the style
void TokenSmall::OnPush( LOGFONT& logFont )
{
  _lfHeight = logFont.lfHeight;
  logFont.lfHeight = logFont.lfHeight / 2;
}

// remove the style
void TokenSmall::OnPop( LOGFONT& logFont )
{
  if (_lfHeight == 0)
  {
    return;
  }
  logFont.lfHeight = _lfHeight;
}
