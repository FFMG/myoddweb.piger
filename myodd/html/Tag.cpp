#include "Tag.h"

namespace myodd{ namespace html{
Tag::Tag() :
  m_depth(0) 
{
}

// apply the style
void Tag::push( LOGFONT& logFont )
{
  OnPush( logFont );
  ++m_depth;
}

// remove the style
void Tag::pop( LOGFONT& logFont )
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
}}