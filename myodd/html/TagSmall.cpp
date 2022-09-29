#include "TagSmall.h"
#include "../string/string.h"

namespace myodd{ namespace html{
TagSmall::TagSmall() :
  Tag(),
  _lfHeight( 0)
{
}

TagSmall::~TagSmall()
{
}

// if this is the Tag we are looking for.
bool TagSmall::IsTag(const wchar_t* lpString, unsigned int nLen) const
{
  if( nLen == 5 )
  {
    return (_tcsnicmp(lpString , L"small", nLen) ==0);
  }
  return false;
}

// apply the style
void TagSmall::OnPush( LOGFONT& logFont )
{
  _lfHeight = logFont.lfHeight;
  logFont.lfHeight = logFont.lfHeight / 2;
}

// remove the style
void TagSmall::OnPop( LOGFONT& logFont )
{
  if (_lfHeight == 0)
  {
    return;
  }
  logFont.lfHeight = _lfHeight;
}
}}