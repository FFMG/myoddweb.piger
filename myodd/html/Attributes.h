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
  void Push(HDC hdc, LOGFONT& logFont);

  // remove the style
  void Pop(HDC hdc, LOGFONT& logFont);

  /**
   * \brief get the number of attributes
   * \return unsigned int the number of attributes
   */
  const unsigned int NumberOfAttributes() const;

  /**
   * \brief add the given attribute to our list of attributes.
   * \param const Attribute& the attribute we are adding to our list.
   * \return unsigned int the number of attributes
   */
  void Add(const Attribute& src);

private:
  std::vector<Attribute*> _attributes;
  void Copy(const Attributes& rhs);
  void Clear();
};
}}