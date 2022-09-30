#pragma once

#include "../common/includes.h"
#include <windows.h>

namespace myodd{ namespace html{
class Tag
{
public:
  Tag();
  virtual ~Tag() = default;

  Tag(const Tag&) = delete;
  void operator=(const Tag&) = delete;

  // if this is the tag we are looking for.
  virtual bool IsTag(const wchar_t* lpString, unsigned int nLen ) const = 0;
  
  // apply the style
  void Push(HDC hdc, LOGFONT& logFont );

  // remove the style
  void Pop(HDC hdc, LOGFONT& logFont );

  virtual bool ToNextLine( bool /* bIsEnd */ ) const{
    return false; // by default we don't
  }

protected:
  // apply the style
  virtual void OnPush(HDC hdc, LOGFONT& logFont ) = 0;

  // remove the style
  virtual void OnPop(HDC hdc, LOGFONT& logFont ) = 0;

private:
  int m_depth;
};
}}