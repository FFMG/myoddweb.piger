#pragma once

#include "../myoddinclude.h"

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

class Token
{
public:
  Token();
  virtual ~Token(){};

  // if this is the token we are looking for.
  bool IsToken( LPCTSTR lpString, int nLen ) const;
  
  // apply the style
  void push( LOGFONT& logFont );

  // remove the style
  void pop( LOGFONT& logFont );

  virtual bool ToNextLine( bool /* bIsEnd */ ) const{
    return false; // by default we don't
  }

protected:
  // apply the style
  virtual void OnPush( LOGFONT& logFont ) = 0;

  // remove the style
  virtual void OnPop( LOGFONT& logFont ) = 0;

  // if this is the token we are looking for.
  virtual bool OnIsToken( LPCTSTR lpString, int nLen ) const = 0;

private:
  int m_depth;

  DISALLOW_COPY_AND_ASSIGN(Token);
};