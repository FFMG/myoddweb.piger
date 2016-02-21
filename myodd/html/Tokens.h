#pragma once

#include <vector>

#include "Token.h"

//
// the tokens
class Tokens : public std::vector<Token*>
{
public:
  Tokens();
  ~Tokens();
};