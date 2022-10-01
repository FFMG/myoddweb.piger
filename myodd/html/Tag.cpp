#include "Tag.h"

namespace myodd{ namespace html{
Tag::Tag(const Attributes& attributes) :
  _depth(0),
  _attributes(attributes)
{
}

Tag::Tag(const Tag& tag)
{
  *this = tag;
}
Tag& Tag::operator=(const Tag& tag)
{
  if (this != &tag)
  {
    _depth = tag._depth;
    _attributes = tag._attributes;
  }
  return *this;
}


// apply the style
void Tag::Push(HDC hdc, LOGFONT& logFont )
{
  // first push the tag itself
  OnPush( hdc, logFont );

  // then the attributes
  _attributes.Push(hdc, logFont);

  ++_depth;
}

// remove the style
void Tag::Pop(HDC hdc, LOGFONT& logFont )
{
  if( _depth >= 1 )
  {
    // pop the attributes
    assert(0);  // we need to pop the parent attribute
                // a closing tag does not have attributes to pop!

    OnPop(hdc, logFont );
    --_depth;
  }
  else
  {
   // *** Trying to pop an un open token ***
  }
}
}}