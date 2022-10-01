#pragma once

#include <vector>

#include "Tag.h"

namespace myodd{ namespace html{
/**
  * \brief a collection of tags.
  */
class Tags final
{
public:
  Tags();
  virtual ~Tags();

  Tags(const Tags& rhs);
  const Tags& operator=(const Tags& rhs);

  // apply the style
  void Push(HDC hdc, LOGFONT& logFont);

  // remove the style
  void Pop(HDC hdc, LOGFONT& logFont);

  /**
   * \brief Add a tag to our list of tags and make sure that the correct tag attributes/style if followed.
   * \param const Tag& the tag we are copying
   */
  void Add(const Tag& src);

  /**
   * \brief get the number of tags
   * \return unsigned int the number of tags
   */
  const unsigned int NumberOfTags() const;

  /**
   * \brief create a tag given the tag name.
   * \param const std::wstring& the tag name we will be looking for.
   * \param const Attributes& the tag attributes
   * \return Tag* or nullptr if the tag cannot be created
   */
  static Tag* CreateFromString(const std::wstring& tagName, const Attributes& attributes);

  /**
   * \brief given a source tag, make a copy of it.
   * \param const Tag& the tag we are trying to create from.
   */
  static Tag* CreateFromSource(const Tag& src);

private:
  std::vector<Tag*> _tags;
  void Copy(const Tags& rhs);
  void Clear();
};
}}