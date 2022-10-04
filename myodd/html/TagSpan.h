#pragma once

#include "Tag.h"

namespace myodd{ namespace html{
class TagSpan : public Tag
{
public:
  TagSpan(const Attributes& attributes, int tagStyle);
  virtual ~TagSpan();

  TagSpan(const TagSpan&);
  TagSpan& operator=(const TagSpan&);

  /**
   * \brief get the current tag type
   * \return the tag type
   */
  virtual Type TagType() const { return Type::Span; }

protected:
  // apply the style
  virtual void OnPush(HDC hdc, LOGFONT& logFont );

  // remove the style
  virtual void OnPop(HDC hdc, LOGFONT& logFont );
};
}}