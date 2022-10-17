#include "TagItalic.h"
#include "../string/string.h"

namespace myodd{ namespace html{
TagItalic::TagItalic(const Attributes& attributes, int tagStyle) :
  Tag(attributes, tagStyle)
{
}

TagItalic::~TagItalic()
{
}

TagItalic::TagItalic(const TagItalic& rhs) : Tag(rhs)
{
  *this = rhs;
}

TagItalic& TagItalic::operator=(const TagItalic& rhs)
{
  if (this != &rhs)
  {
    //  copy
    Tag::operator=(rhs);
  }
  return *this;
}

// apply the style
void TagItalic::OnPush(HDC hdc, LOGFONT& logFont )
{
  UNUSED_ALWAYS(hdc);
  logFont.lfItalic = 1;
}

// remove the style
void TagItalic::OnPop(HDC hdc, LOGFONT& logFont )
{
  UNUSED_ALWAYS(hdc);
  logFont.lfItalic = 0;
}
}}