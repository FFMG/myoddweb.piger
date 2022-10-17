#pragma once

#include "Tag.h"

namespace myodd{ namespace html{
class TagItalic : public Tag
{
public:
  TagItalic(const Attributes& attributes, int tagStyle);
  virtual ~TagItalic();

  TagItalic(const TagItalic&);
  TagItalic& operator=(const TagItalic&);

  /**
   * \brief get the current tag type
   * \return the tag type
   */
  virtual Type TagType() const { return Type::Italic; }

protected:
  // apply the style
  virtual void OnPush(HDC hdc, LOGFONT& logFont );

  // remove the style
  virtual void OnPop(HDC hdc, LOGFONT& logFont );
};
}}