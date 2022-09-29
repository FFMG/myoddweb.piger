#include "TagItalic.h"
#include "../string/string.h"

namespace myodd{ namespace html{
TagItalic::TagItalic() :
  Tag()
{
}

TagItalic::~TagItalic()
{
}

// if this is the Tag we are looking for.
bool TagItalic::IsTag(const wchar_t* lpString, unsigned int nLen) const
{
  if( nLen == 1 )
  {
    return (_tcsnicmp(lpString , L"i", nLen) ==0);
  }
  if( nLen == 2 )
  {
    return (_tcsnicmp(lpString , L"em", nLen) ==0);
  }
  return false;
}

// apply the style
void TagItalic::OnPush( LOGFONT& logFont )
{
  logFont.lfItalic = 1;
}

// remove the style
void TagItalic::OnPop( LOGFONT& logFont )
{
  logFont.lfItalic = 0;
}
}}