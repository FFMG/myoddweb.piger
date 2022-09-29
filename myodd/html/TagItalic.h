#pragma once

#include "Tag.h"

namespace myodd{ namespace html{
class TagItalic : public Tag
{
public:
  TagItalic();
  ~TagItalic();

protected:
  // apply the style
  virtual void OnPush( LOGFONT& logFont );

  // remove the style
  virtual void OnPop( LOGFONT& logFont );

  // if this is the Tag we are looking for.
  virtual bool OnIsTag(const wchar_t* lpString, __int64 nLen ) const;
private:
};
}}