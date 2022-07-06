#include "Token.h"

Token::Token() : 
  m_depth(0) 
{
}

// apply the style
void Token::push( LOGFONT& logFont )
{
  OnPush( logFont );
  ++m_depth;
}

// remove the style
void Token::pop( LOGFONT& logFont )
{
  if( m_depth >= 1 )
  {
    OnPop( logFont );
    --m_depth;
  }
  else
  {
   // *** Trying to pop an un open token ***
  }
}

bool Token::IsToken(const wchar_t* lpString, __int64 nLen ) const
{
  if( !lpString )
  {
    return false;
  }
  return OnIsToken( lpString, nLen );
}