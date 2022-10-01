#pragma once

#include "Tag.h"

namespace myodd{ namespace html{
class TagItalic : public Tag
{
public:
  TagItalic(const Attributes& attributes);
  virtual ~TagItalic();

  TagItalic(const TagItalic&);
  TagItalic& operator=(const TagItalic&);

protected:
  // apply the style
  virtual void OnPush(HDC hdc, LOGFONT& logFont );

  // remove the style
  virtual void OnPop(HDC hdc, LOGFONT& logFont );
};
}}