#pragma once
#include "../common/includes.h"
#include <vector>
#include "AttributeValue.h"

namespace myodd{ namespace html{
class Attribute
{
public:
  Attribute();
  virtual ~Attribute();

  Attribute(const Attribute&);
  Attribute& operator=(const Attribute&);
    
  // apply the style
  virtual void Push( LOGFONT& logFont ) = 0;

  // remove the style
  virtual void Pop( LOGFONT& logFont ) = 0;

  /**
   * \brief add a single attribute value
   * \param const AttributeValue& the value we are adding.
   */
  void Add(const AttributeValue& value);

protected:
  // copy the attribute values here.
  virtual void Copy(const Attribute& rhs);

  void Clear();

private:
  // all the values.
  std::vector<AttributeValue*> _values;
};
}}