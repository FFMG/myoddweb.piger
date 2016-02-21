#include "Tokens.h"

#include "TokenBold.h"
#include "TokenItalic.h"
#include "TokenUnderline.h"
#include "TokenBr.h"
#include "TokenStrikeout.h"

Tokens::Tokens()
{
  push_back( new TokenBold );
  push_back( new TokenItalic );
  push_back( new TokenUnderline );
  push_back( new TokenBr );
  push_back( new TokenStrikeout );
}

Tokens::~Tokens()
{
  for( const_iterator it = begin();
    it != end();
    it++ )
  {
    delete *it;
  }
}
