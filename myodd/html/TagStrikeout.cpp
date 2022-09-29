#include "TagStrikeout.h"
#include "../string/string.h"

namespace myodd{ namespace html{
TagStrikeout::TagStrikeout() :
Tag()
{
}

TagStrikeout::~TagStrikeout()
{
}

// if this is the Tag we are looking for.
bool TagStrikeout::OnIsTag(const wchar_t* lpString, __int64 nLen ) const
{
  if( nLen == 1 )
  {
    return (_tcsnicmp(lpString , L"s", nLen) ==0);
  }
  if( nLen == 9 )
  {
    return (_tcsnicmp(lpString , L"strikeout", nLen) ==0);
  }
  return false;
}

// apply the style
void TagStrikeout::OnPush( LOGFONT& logFont )
{
  logFont.lfStrikeOut = 1;
}

// remove the style
void TagStrikeout::OnPop( LOGFONT& logFont )
{
  logFont.lfStrikeOut = 0;
}
}}