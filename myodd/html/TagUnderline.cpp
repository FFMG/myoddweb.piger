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

// if this is the Tag we are looking for.
bool TagUnderline::IsTag(const wchar_t* lpString, unsigned int nLen) const
{
  if( nLen == 1 )
  {
    return (_tcsnicmp(lpString , L"u", nLen) ==0);
  }
  return false;
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