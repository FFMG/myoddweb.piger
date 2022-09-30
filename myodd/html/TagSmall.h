#pragma once

#include "Tag.h"

namespace myodd{ namespace html{
class TagSmall : public Tag
{
public:
  TagSmall();
  ~TagSmall();

  // if this is the Tag we are looking for.
  virtual bool IsTag(const wchar_t* lpString, unsigned int nLen) const;

protected:
  // apply the style
  virtual void OnPush(HDC hdc, LOGFONT& logFont );

  // remove the style
  virtual void OnPop(HDC hdc, LOGFONT& logFont );

private:
  long _lfHeight;
};
}}