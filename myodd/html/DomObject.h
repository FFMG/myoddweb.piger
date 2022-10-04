#pragma once
namespace myodd { namespace html {
class DomObject
{
protected:
  DomObject();
  DomObject( const DomObject& );
  DomObject& operator=(const DomObject& rhs);

public:
  virtual ~DomObject();
};
}}
