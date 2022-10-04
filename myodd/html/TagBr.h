#pragma once

#include "Tag.h"

namespace myodd{ namespace html{
class TagBr : public Tag
{
public:
  TagBr(const Attributes& attributes, int tagStyle);
  virtual ~TagBr();

  TagBr(const TagBr&);
  TagBr& operator=(const TagBr&);

  virtual bool ToNextLine() const;

  /**
   * \brief get the current tag type
   * \return the tag type
   */
  virtual Type TagType() const { return Type::Br; }

protected:
  // apply the style
  virtual void OnPush(HDC hdc, LOGFONT& logFont );

  // remove the style
  virtual void OnPop(HDC hdc, LOGFONT& logFont );
};
}}