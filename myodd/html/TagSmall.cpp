#include "TagSmall.h"
#include "../string/string.h"

namespace myodd{ namespace html{
TagSmall::TagSmall(const Attributes& attributes, int tagType) :
  Tag(attributes, tagType),
  _lfHeight( 0)
{
}

TagSmall::~TagSmall()
{
}

TagSmall::TagSmall(const TagSmall& rhs) : Tag(rhs)
{
  *this = rhs;
}

TagSmall& TagSmall::operator=(const TagSmall& rhs)
{
  if (this != &rhs)
  {
    //  copy
    Tag::operator=(rhs);
    _lfHeight = rhs._lfHeight;
  }
  return *this;
}

// apply the style
void TagSmall::OnPush(HDC hdc, LOGFONT& logFont )
{
  UNUSED_ALWAYS(hdc);
  _lfHeight = logFont.lfHeight;
  logFont.lfHeight = logFont.lfHeight / 2;
}

// remove the style
void TagSmall::OnPop(HDC hdc, LOGFONT& logFont )
{
  UNUSED_ALWAYS(hdc);
  if (_lfHeight == 0)
  {
    return;
  }
  logFont.lfHeight = _lfHeight;
}
}}