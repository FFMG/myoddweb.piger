#include "TagBold.h"
#include "../string/string.h"

namespace myodd{ namespace html{
TagBold::TagBold() :
  Tag()
{
}

TagBold::~TagBold()
{
}

// if this is the Tag we are looking for.
bool TagBold::IsTag(const wchar_t* lpString, unsigned int nLen) const
{
  if( nLen == 1 )
  {
    return (_tcsnicmp(lpString , L"b", nLen) ==0);
  }
  if( nLen == 6 )
  {
    return (_tcsnicmp(lpString , L"strong", nLen) ==0);
  }
  return false;
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