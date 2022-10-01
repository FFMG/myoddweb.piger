#include "TagUnderline.h"
#include "../string/string.h"

namespace myodd{ namespace html{
TagUnderline::TagUnderline() :
  Tag()
{
}

TagUnderline::~TagUnderline()
{
}

TagUnderline::TagUnderline(const TagUnderline& rhs) : Tag(rhs)
{
  *this = rhs;
}

TagUnderline& TagUnderline::operator=(const TagUnderline& rhs)
{
  if (this != &rhs)
  {
    //  copy
    Tag::operator=(rhs);
  }
  return *this;
}

// apply the style
void TagUnderline::OnPush(HDC hdc, LOGFONT& logFont )
{
  UNUSED_ALWAYS(hdc);
  logFont.lfUnderline = 1;
}

// remove the style
void TagUnderline::OnPop(HDC hdc, LOGFONT& logFont )
{
  UNUSED_ALWAYS(hdc);
  logFont.lfUnderline = 0;
}
}}