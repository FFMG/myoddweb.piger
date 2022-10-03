#include "TagStrikeout.h"
#include "../string/string.h"

namespace myodd{ namespace html{
TagStrikeout::TagStrikeout(const Attributes& attributes, int tagStyle) :
Tag(attributes, tagStyle)
{
}

TagStrikeout::~TagStrikeout()
{
}

TagStrikeout::TagStrikeout(const TagStrikeout& rhs) : Tag(rhs)
{
  *this = rhs;
}

TagStrikeout& TagStrikeout::operator=(const TagStrikeout& rhs)
{
  if (this != &rhs)
  {
    //  copy
    Tag::operator=(rhs);
  }
  return *this;
}

// apply the style
void TagStrikeout::OnPush(HDC hdc, LOGFONT& logFont )
{
  UNUSED_ALWAYS(hdc);
  logFont.lfStrikeOut = 1;
}

// remove the style
void TagStrikeout::OnPop(HDC hdc, LOGFONT& logFont )
{
  UNUSED_ALWAYS(hdc);
  logFont.lfStrikeOut = 0;
}
}}