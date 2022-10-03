#pragma once
#include <vector>
#include "Tag.h"
#include "DomObject.h"

namespace myodd { namespace html {
class DomObjects : public std::vector<DomObject*>
{
public:
  DomObjects() = default;

  DomObjects( const DomObjects& );
  DomObjects& operator=( const DomObjects& );

  /**
   * \brief delete all the DomObjects and clear the array.
   */
  virtual ~DomObjects();

  void AddTag(const Tag& src);
  void AddContent(const std::wstring& content);
  void Clear();

private:
  std::wstring EscapeText(const std::wstring& src) const;
};
}}
