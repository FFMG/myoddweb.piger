#include "Tags.h"

#include "..\string\string.h"

namespace myodd{ namespace html{
Tags::Tags()
{
}

Tags::~Tags()
{
  Clear();
}

Tags::Tags(const Tags& rhs)
{
  Copy(rhs);
}

const Tags& Tags::operator=(const Tags& rhs)
{
  Copy(rhs);
  return *this;
}

void Tags::Clear()
{
  for (auto tag : _tags)
  {
    delete tag;
  }
  _tags.clear();
}

void Tags::Copy(const Tags& rhs)
{
  if (this == &rhs)
  {
    return;
  }
  Clear();

  for (auto tag : rhs._tags)
  {
    Add(*tag);
  }
}

/**
 * \brief get the number of tags
 * \return unsigned int the number of tags
 */
const unsigned int Tags::NumberOfTags() const
{
  return static_cast<unsigned int>(_tags.size());
}

/**
 * \brief Add a start tag to our list of tags and make sure that the correct tag attributes/style if followed.
 * \param const Tag& the tag we are copying
 */
void Tags::Add(const Tag& src)
{
  auto clone = Tag::CreateFromSource(src);
  if (clone != nullptr)
  {
    _tags.push_back(clone);
  }
}
}}