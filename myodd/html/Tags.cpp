#include "Tags.h"

#include "TagSmall.h"
#include "TagBold.h"
#include "TagItalic.h"
#include "TagUnderline.h"
#include "TagBr.h"
#include "TagStrikeout.h"

namespace myodd{ namespace html{
Tags::Tags()
{
  push_back(new TagBold );
  push_back(new TagItalic );
  push_back(new TagUnderline );
  push_back(new TagBr );
  push_back(new TagStrikeout );
  push_back(new TagSmall);
}

Tags::~Tags()
{
  for( const_iterator it = begin(); it != end(); it++ )
  {
    delete *it;
  }
}
}}