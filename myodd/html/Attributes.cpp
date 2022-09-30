#include "Attributes.h"

namespace myodd{ namespace html{
Attributes::Attributes()
{
}

Attributes::~Attributes()
{
  Clear();
}

Attributes::Attributes(const Attributes& rhs)
{
  Copy(rhs);
}

const Attributes& Attributes::operator=(const Attributes& rhs)
{
  Copy(rhs);
  return* this;
}

void Attributes::Clear()
{
  for (auto att : _attributes)
  {
    delete att;
  }
  _attributes.clear();
}

void Attributes::Copy(const Attributes& rhs)
{
  if (this == &rhs)
  {
    return;
  }
  Clear();

  for (auto att : rhs._attributes)
  {
    assert(0);  //  we need to add each kind.
  }
}

/**
 * \brief get the number of attributes
 * \return unsigned int the number of attributes
 */
const unsigned int Attributes::NumberOfAttributes() const
{
  return static_cast<unsigned int>(_attributes.size());
}


// apply the style
void Attributes::Push(LOGFONT& logFont)
{
  //  we push from start to end
  for (auto att : _attributes)
  {
    att->Push(logFont);
  }
}

// remove the style
void Attributes::Pop(LOGFONT& logFont)
{
  // we pop in reverse order
  for (auto it = _attributes.rbegin(); it != _attributes.rend(); it++)
  {
    (*it)->Pop(logFont);
  }
}

}}