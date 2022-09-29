#pragma once

#include <vector>

#include "Tag.h"

namespace myodd{ namespace html{
//
// the Tags
class Tags : public std::vector<Tag*>
{
public:
  Tags();
  ~Tags();
};
}}