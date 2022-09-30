#pragma once

#include "../common/includes.h"
#include <windows.h>

namespace myodd{ namespace html{
class Attribute
{
public:
  Attribute();
  virtual ~Attribute() = default;

  Attribute(const Attribute&);
  Attribute& operator=(const Attribute&);

  // if this is the tag we are looking for.
  virtual bool IsAttribute(const wchar_t* lpString, unsigned int nLen ) const = 0;
  
  // apply the style
  virtual void Push( LOGFONT& logFont ) = 0;

  // remove the style
  virtual void Pop( LOGFONT& logFont ) = 0;

protected:
  // copy the attribute values here.
  virtual void Copy(const Attribute& rhs);
};
}}