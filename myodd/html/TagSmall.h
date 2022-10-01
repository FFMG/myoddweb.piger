#pragma once

#include "Tag.h"

namespace myodd{ namespace html{
class TagSmall : public Tag
{
public:
  TagSmall(const Attributes& attributes);
  virtual ~TagSmall();

  TagSmall(const TagSmall&);
  TagSmall& operator=(const TagSmall&);

protected:
  // apply the style
  virtual void OnPush(HDC hdc, LOGFONT& logFont );

  // remove the style
  virtual void OnPop(HDC hdc, LOGFONT& logFont );

private:
  long _lfHeight;
};
}}