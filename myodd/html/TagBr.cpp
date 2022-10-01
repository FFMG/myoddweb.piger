#include "TagBr.h"
#include "../string/string.h"

namespace myodd{ namespace html{
TagBr::TagBr(const Attributes& attributes) :
  Tag(attributes)
{
}

TagBr::~TagBr()
{
}

TagBr::TagBr(const TagBr& rhs) : Tag(rhs)
{
  *this = rhs;
}

TagBr& TagBr::operator=(const TagBr& rhs)
{
  if (this != &rhs)
  {
    //  copy
    Tag::operator=(rhs);
  }
  return *this;
}

bool TagBr::ToNextLine( bool bIsEnd ) const
{
  UNUSED_ALWAYS( bIsEnd );
  return true;
}

void TagBr::OnPush(HDC hdc, LOGFONT& logFont )
{
  //  we do nothing
  UNUSED_ALWAYS( logFont );
  UNUSED_ALWAYS(hdc);
}

void TagBr::OnPop(HDC hdc, LOGFONT& logFont )
{
  //  we do nothing
  UNUSED_ALWAYS( logFont );
  UNUSED_ALWAYS(hdc);
}
}}