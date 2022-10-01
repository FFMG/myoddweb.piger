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

void Tags::Add(const Tag& src)
{
  auto tagBold = dynamic_cast<const TagBold*>(&src);
  if (nullptr != tagBold)
  {
    _tags.push_back(new TagBold(*tagBold));
    return;
  }

  auto tagBr = dynamic_cast<const TagBr*>(&src);
  if (nullptr != tagBr)
  {
    _tags.push_back(new TagBr(*tagBr));
    return;
  }

  auto tagItalic = dynamic_cast<const TagItalic*>(&src);
  if (nullptr != tagItalic)
  {
    _tags.push_back(new TagItalic(*tagItalic));
    return;
  }

  auto tagSmall = dynamic_cast<const TagSmall*>(&src);
  if (nullptr != tagSmall)
  {
    _tags.push_back(new TagSmall(*tagSmall));
    return;
  }

  auto tagStrikeout = dynamic_cast<const TagStrikeout*>(&src);
  if (nullptr != tagStrikeout)
  {
    _tags.push_back(new TagStrikeout(*tagStrikeout));
    return;
  }

  auto tagUnderline = dynamic_cast<const TagUnderline*>(&src);
  if (nullptr != tagUnderline)
  {
    _tags.push_back(new TagUnderline(*tagUnderline));
    return;
  }
}

/**
 * \brief create a tag given the tag name.
 * \param const std::wstring& the tag name we will be looking for.
 * \return Tag* or nullptr if the tag cannot be created
 */
Tag* Tags::CreateFromString(const std::wstring& tagName)
{
  auto nLen = tagName.length();
  auto lowerTagName = myodd::strings::lower(tagName);
  auto lpString = lowerTagName.c_str();
  switch (nLen)
  {
  case 1:
    if (_tcsncmp(lpString, L"b", nLen) == 0)
    {
      return new TagBold();
    }
    if (_tcsncmp(lpString, L"i", nLen) == 0)
    {
      return new TagItalic();
    }
    if (_tcsncmp(lpString, L"s", nLen) == 0)
    {
      return new TagStrikeout();
    }
    if (_tcsncmp(lpString, L"u", nLen) == 0)
    {
      return new TagUnderline();
    }
    break;

  case 2:
    if (_tcsncmp(lpString, L"br", nLen) == 0)
    {
      return new TagBr();
    }
    if (_tcsncmp(lpString, L"em", nLen) == 0)
    {
      return new TagItalic();
    }
    break;

  case 5:
    if (_tcsncmp(lpString, L"small", nLen) == 0)
    {
      return new TagSmall();
    }
    break;

  case 6:
    if (_tcsncmp(lpString, L"strong", nLen) == 0)
    {
      return new TagBold();
    }
    break;

  case 9:
    if (_tcsncmp(lpString, L"strikeout", nLen) == 0)
    {
      return new TagStrikeout();
    }
    break;

  default:
    return false;
  }
  return false;
}

}}