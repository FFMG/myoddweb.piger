#include "DomObject.h"

namespace myodd { namespace html {
DomObject::DomObject()
{
}

DomObject::DomObject(const DomObject& rhs)
{
  *this = rhs;
}

DomObject& DomObject::operator=(const DomObject& rhs)
{
  if (this != &rhs)
  {
    // copy values.
  }
  return *this;
}

DomObject::~DomObject()
{
}
}}