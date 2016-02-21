#pragma once

#include "Token.h"

class TokenBr : public Token
{
public:
  TokenBr();
  ~TokenBr();

  virtual bool ToNextLine( bool bIsEnd ) const;
protected:
  // apply the style
  virtual void OnPush( LOGFONT& logFont );

  // remove the style
  virtual void OnPop( LOGFONT& logFont );

  // if this is the token we are looking for.
  virtual bool OnIsToken( LPCTSTR lpString, int nLen ) const;
};