#pragma once

#include <vector>

#include "Tag.h"

namespace myodd{ namespace html{
//
// the Tags
class Tags : protected std::vector<Tag*>
{
public:
  Tags();
  ~Tags();

  Tag* FindTag(const std::wstring& text) const;
};
}}