#pragma once

#include "Token.h"

class TokenStrikeout : public Token
{
public:
  TokenStrikeout();
  ~TokenStrikeout();

protected:
  // apply the style
  virtual void OnPush( LOGFONT& logFont );

  // remove the style
  virtual void OnPop( LOGFONT& logFont );

  // if this is the token we are looking for.
  virtual bool OnIsToken(const MYODD_CHAR* lpString, int nLen ) const;
};