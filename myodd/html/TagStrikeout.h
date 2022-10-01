#pragma once

#include "Tag.h"

namespace myodd{ namespace html{
class TagStrikeout : public Tag
{
public:
  TagStrikeout(const Attributes& attributes, int tagType);
  virtual ~TagStrikeout();

  TagStrikeout(const TagStrikeout&);
  TagStrikeout& operator=(const TagStrikeout&);

protected:
  // apply the style
  virtual void OnPush(HDC hdc, LOGFONT& logFont );

  // remove the style
  virtual void OnPop(HDC hdc, LOGFONT& logFont );
};
}}