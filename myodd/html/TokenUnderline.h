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
  virtual bool OnIsToken(const wchar_t* lpString, __int64 nLen ) const;
};