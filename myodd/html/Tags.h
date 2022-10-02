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

private:
  std::vector<Tag*> _tags;
  void Copy(const Tags& rhs);
  void Clear();
};
}}