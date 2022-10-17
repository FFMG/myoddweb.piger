#include "TagSpan.h"
#include "../string/string.h"

namespace myodd{ namespace html{
TagSpan::TagSpan(const Attributes& attributes, int tagStyle) :
  Tag(attributes, tagStyle)
{
}

TagSpan::~TagSpan()
{
}

TagSpan::TagSpan(const TagSpan& rhs) : Tag( rhs )
{
  *this = rhs;
}

TagSpan& TagSpan::operator=(const TagSpan& rhs)
{
  if (this != &rhs)
  {
    //  copy
    Tag::operator=(rhs);
  }
  return *this;
}

// apply the style
void TagSpan::OnPush(HDC hdc, LOGFONT& logFont )
{
  UNUSED_ALWAYS(hdc);
  UNUSED_ALWAYS(logFont);
}

// remove the style
void TagSpan::OnPop(HDC hdc, LOGFONT& logFont )
{
  UNUSED_ALWAYS(hdc);
  UNUSED_ALWAYS(logFont);
}
}}