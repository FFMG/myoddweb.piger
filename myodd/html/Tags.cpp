#include "Tags.h"

#include "..\string\string.h"
#include "TagSmall.h"
#include "TagBold.h"
#include "TagItalic.h"
#include "TagUnderline.h"
#include "TagBr.h"
#include "TagStrikeout.h"

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
  auto clone = CreateFromSource(src);
  if (clone != nullptr)
  {
    _tags.push_back(clone);
  }
}

/**
 * \brief given a source tag, make a copy of it.
 * \param const Tag& the tag we are trying to create from.
 */
Tag* Tags::CreateFromSource(const Tag& src)
{
  auto tagBold = dynamic_cast<const TagBold*>(&src);
  if (nullptr != tagBold)
  {
    return new TagBold(*tagBold);
  }

  auto tagBr = dynamic_cast<const TagBr*>(&src);
  if (nullptr != tagBr)
  {
    return new TagBr(*tagBr);
  }

  auto tagItalic = dynamic_cast<const TagItalic*>(&src);
  if (nullptr != tagItalic)
  {
    return new TagItalic(*tagItalic);
  }

  auto tagSmall = dynamic_cast<const TagSmall*>(&src);
  if (nullptr != tagSmall)
  {
    return new TagSmall(*tagSmall);
  }

  auto tagStrikeout = dynamic_cast<const TagStrikeout*>(&src);
  if (nullptr != tagStrikeout)
  {
    return new TagStrikeout(*tagStrikeout);
  }

  auto tagUnderline = dynamic_cast<const TagUnderline*>(&src);
  if (nullptr != tagUnderline)
  {
    return new TagUnderline(*tagUnderline);
  }

  return nullptr;
}

/**
 * \brief create a tag given the tag name.
 * \param const std::wstring& the tag name we will be looking for.
 * \param const Attributes& the tag attributes
 * \return Tag* or nullptr if the tag cannot be created
 */
Tag* Tags::CreateFromString(const std::wstring& tagName, const Attributes& attributes)
{
  auto nLen = tagName.length();
  auto lowerTagName = myodd::strings::lower(tagName);
  auto lpString = lowerTagName.c_str();
  switch (nLen)
  {
  case 1:
    if (_tcsncmp(lpString, L"b", nLen) == 0)
    {
      return new TagBold(attributes);
    }
    if (_tcsncmp(lpString, L"i", nLen) == 0)
    {
      return new TagItalic(attributes);
    }
    if (_tcsncmp(lpString, L"s", nLen) == 0)
    {
      return new TagStrikeout(attributes);
    }
    if (_tcsncmp(lpString, L"u", nLen) == 0)
    {
      return new TagUnderline(attributes);
    }
    break;

  case 2:
    if (_tcsncmp(lpString, L"br", nLen) == 0)
    {
      return new TagBr(attributes);
    }
    if (_tcsncmp(lpString, L"em", nLen) == 0)
    {
      return new TagItalic(attributes);
    }
    break;

  case 5:
    if (_tcsncmp(lpString, L"small", nLen) == 0)
    {
      return new TagSmall(attributes);
    }
    break;

  case 6:
    if (_tcsncmp(lpString, L"strong", nLen) == 0)
    {
      return new TagBold(attributes);
    }
    break;

  case 9:
    if (_tcsncmp(lpString, L"strikeout", nLen) == 0)
    {
      return new TagStrikeout(attributes);
    }
    break;

  default:
    return false;
  }
  return false;
}

}}