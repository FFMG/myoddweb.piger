#pragma once

#include "Tag.h"

namespace myodd{ namespace html{
class TagBold : public Tag
{
public:
  TagBold(const Attributes& attributes, int tagStyle);
  virtual ~TagBold();

  TagBold(const TagBold&);
  TagBold& operator=(const TagBold&);

  /**
   * \brief get the current tag type
   * \return the tag type
   */
  virtual Type TagType() const { return Type::Bold; }

protected:
  // apply the style
  virtual void OnPush(HDC hdc, LOGFONT& logFont );

  // remove the style
  virtual void OnPop(HDC hdc, LOGFONT& logFont );
};
}}