#pragma once

#include "Tag.h"

namespace myodd{ namespace html{
class TagUnderline : public Tag
{
public:
  TagUnderline(const Attributes& attributes, int tagType);
  virtual ~TagUnderline();

  TagUnderline(const TagUnderline&);
  TagUnderline& operator=(const TagUnderline&);

protected:
  // apply the style
  virtual void OnPush(HDC hdc, LOGFONT& logFont );

  // remove the style
  virtual void OnPop(HDC hdc, LOGFONT& logFont );
};
}}