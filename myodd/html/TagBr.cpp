#include "TagBr.h"
#include "../string/string.h"

namespace myodd{ namespace html{
TagBr::TagBr() :
  Tag()
{
}

TagBr::~TagBr()
{
}

bool TagBr::ToNextLine( bool bIsEnd ) const
{
  UNUSED_ALWAYS( bIsEnd );
  return true;
}

// if this is the Tag we are looking for.
bool TagBr::IsTag(const wchar_t* lpString, unsigned int nLen ) const
{
  switch (nLen)
  {
  case 2:
    return (_tcsnicmp(lpString, L"br", nLen) == 0);

  default:
    return false;
  }
}

void TagBr::OnPush(HDC hdc, LOGFONT& logFont )
{
  //  we do nothing
  UNUSED_ALWAYS( logFont );
  UNUSED_ALWAYS(hdc);
}

void TagBr::OnPop(HDC hdc, LOGFONT& logFont )
{
  //  we do nothing
  UNUSED_ALWAYS( logFont );
  UNUSED_ALWAYS(hdc);
}
}}