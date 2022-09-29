#include "Tags.h"

#include "TagSmall.h"
#include "TagBold.h"
#include "TagItalic.h"
#include "TagUnderline.h"
#include "TagBr.h"
#include "TagStrikeout.h"

namespace myodd{ namespace html{
Tags::Tags()
{
  push_back(new TagBold );
  push_back(new TagItalic );
  push_back(new TagUnderline );
  push_back(new TagBr );
  push_back(new TagStrikeout );
  push_back(new TagSmall);
}

Tags::~Tags()
{
  for( const_iterator it = begin(); it != end(); it++ )
  {
    delete *it;
  }
}

/**
 * \brief given a name look for a tag in the list of possible tags
 * \param const std::wstring& the tag we are looking for.
 * \return either null or the tag
 */
Tag* Tags::FindTag(const std::wstring& text) const
{
  const auto givenText = text.c_str();
  const auto givenTextLength = text.length();
  for (const auto tag : *this)
  {
    if (tag->IsTag(givenText, givenTextLength))
    {
      return tag;
    }
  }
  return nullptr;
}
}}