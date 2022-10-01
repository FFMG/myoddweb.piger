#pragma once

#include "Tag.h"

namespace myodd{ namespace html{
class TagBr : public Tag
{
public:
  TagBr(const Attributes& attributes, int tagType);
  virtual ~TagBr();

  TagBr(const TagBr&);
  TagBr& operator=(const TagBr&);

  virtual bool ToNextLine() const;

protected:
  // apply the style
  virtual void OnPush(HDC hdc, LOGFONT& logFont );

  // remove the style
  virtual void OnPop(HDC hdc, LOGFONT& logFont );
};
}}