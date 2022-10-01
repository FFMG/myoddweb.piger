#pragma once

#include "../common/includes.h"
#include "Attributes.h"

namespace myodd{ namespace html{
class Tag
{
public:
  Tag( const Attributes& attributes );
  virtual ~Tag() = default;

  Tag(const Tag&);
  Tag& operator=(const Tag&);
  
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
  /** 
   * \brief the depth of this tag
   */
  int _depth;

  /**
   * \brief the tag attribute
   */
  Attributes _attributes;
};
}}