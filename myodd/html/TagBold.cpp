#include "TagBold.h"
#include "../string/string.h"

namespace myodd{ namespace html{
TagBold::TagBold(const Attributes& attributes, int tagType) :
  Tag(attributes, tagType )
{
}

TagBold::~TagBold()
{
}

TagBold::TagBold(const TagBold& rhs) : Tag( rhs )
{
  *this = rhs;
}

TagBold& TagBold::operator=(const TagBold& rhs)
{
  if (this != &rhs)
  {
    //  copy
    Tag::operator=(rhs);
  }
  return *this;
}

// apply the style
void TagBold::OnPush(HDC hdc, LOGFONT& logFont )
{
  UNUSED_ALWAYS(hdc);
  logFont.lfWeight = FW_BOLD;
}

// remove the style
void TagBold::OnPop(HDC hdc, LOGFONT& logFont )
{
  UNUSED_ALWAYS(hdc);
  logFont.lfWeight = FW_NORMAL;
}
}}