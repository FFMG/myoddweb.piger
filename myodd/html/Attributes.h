#pragma once

#include <vector>

#include "Attribute.h"

namespace myodd{ namespace html{
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

  /**
   * \brief get the number of attributes
   * \return unsigned int the number of attributes
   */
  const unsigned int NumberOfAttributes() const;

private:
  std::vector<Attribute*> _attributes;
  void Copy(const Attributes& rhs);
  void Clear();
};
}}