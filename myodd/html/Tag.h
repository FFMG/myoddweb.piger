#pragma once

#include "../common/includes.h"
#include "Attributes.h"

namespace myodd{ namespace html{
class Tag
{
public:
  enum Type
  {
    None    = 0,
    Opening = 1,
    Closing = 2
  };

  Tag( const Attributes& attributes, int tagType );
  virtual ~Tag() = default;

  Tag(const Tag&);
  Tag& operator=(const Tag&);
  
  // apply the style
  void Push(HDC hdc, LOGFONT& logFont );

  // remove the style
  void Pop(HDC hdc, LOGFONT& logFont );

  virtual bool ToNextLine() const{
    return false; // by default we don't
  }

  /**
   * \brief check if something of a given type.
   * \param const Type the type we are checking
   * \return bool if the flag we are checking is set
   */
  bool Is(Type type) const;

  /**
   * \brief create a tag given the tag name.
   * \param const std::wstring& the tag name we will be looking for.
   * \param const Attributes& the tag attributes
   * \param const int& the type flag, (open/closed etc)
   * \return Tag* or nullptr if the tag cannot be created
   */
  static Tag* CreateFromString(const std::wstring& tagName, const Attributes& attributes, const int& tagType);

  /**
   * \brief given a source tag, make a copy of it.
   * \param const Tag& the tag we are trying to create from.
   */
  static Tag* CreateFromSource(const Tag& src);

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

  /**
   * \brief the tag type.
   */
  int _tagType;
};
}}