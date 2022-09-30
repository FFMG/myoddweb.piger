#include "Tag.h"

namespace myodd{ namespace html{
Tag::Tag() :
  m_depth(0) 
{
}

// apply the style
void Tag::Push(HDC hdc, LOGFONT& logFont )
{
  OnPush( hdc, logFont );
  ++m_depth;
}

// remove the style
void Tag::Pop(HDC hdc, LOGFONT& logFont )
{
  if( m_depth >= 1 )
  {
    OnPop(hdc, logFont );
    --m_depth;
  }
  else
  {
   // *** Trying to pop an un open token ***
  }
}
}}