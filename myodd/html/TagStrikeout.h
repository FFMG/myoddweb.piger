#pragma once

#include "Tag.h"

namespace myodd{ namespace html{
class TagStrikeout : public Tag
{
public:
  TagStrikeout(const Attributes& attributes, int tagStyle);
  virtual ~TagStrikeout();

  TagStrikeout(const TagStrikeout&);
  TagStrikeout& operator=(const TagStrikeout&);

  /**
   * \brief get the current tag type
   * \return the tag type
   */
  virtual Type TagType() const { return Type::StrikeOut; }

protected:
  // apply the style
  virtual void OnPush(HDC hdc, LOGFONT& logFont );

  // remove the style
  virtual void OnPop(HDC hdc, LOGFONT& logFont );
};
}}