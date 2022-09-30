#pragma once

#include <vector>

#include "Attribute.h"

namespace myodd{ namespace html{
//
// the Tags
class Attributes final
{
public:
  Attributes();
  virtual ~Attributes();

  Attributes(const Attributes& rhs);
  const Attributes& operator=(const Attributes& rhs);

  // apply the style
  void Push(LOGFONT& logFont);

  // remove the style
  void Pop(LOGFONT& logFont);

private:
  std::vector<Attribute*> _attributes;
  void Copy(const Attributes& rhs);
  void Clear();
};
}}