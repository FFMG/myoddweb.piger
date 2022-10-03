#pragma once
#include "../string/string.h"
#include "Tag.h"

namespace myodd { namespace html {
class DomObject
{
protected:
  DomObject();
  DomObject( const DomObject& );
public:
  DomObject& operator=(const DomObject& rhs);

  virtual ~DomObject();
};
}}
