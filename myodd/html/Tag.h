#pragma once

#include "../common/includes.h"
#include <windows.h>

namespace myodd{ namespace html{
class Tag
{
public:
  Tag();
  virtual ~Tag(){};

  Tag(const Tag&) = delete;
  void operator=(const Tag&) = delete;

  // if this is the tag we are looking for.
  bool IsTag(const wchar_t* lpString, __int64 nLen ) const;
  
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

  // if this is the tag we are looking for.
  virtual bool OnIsTag(const wchar_t* lpString, __int64 nLen ) const = 0;

private:
  int m_depth;
};
}}