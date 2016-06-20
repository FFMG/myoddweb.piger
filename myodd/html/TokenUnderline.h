#pragma once

#include "Token.h"

class TokenUnderline : public Token
{
public:
  TokenUnderline();
  ~TokenUnderline();

protected:
  // apply the style
  virtual void OnPush( LOGFONT& logFont );

  // remove the style
  virtual void OnPop( LOGFONT& logFont );

  // if this is the token we are looking for.
  virtual bool OnIsToken(const MYODD_CHAR* lpString, __int64 nLen ) const;
};