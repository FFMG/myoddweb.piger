#include "..\string\string.h"
#include "Tag.h"
#include "TagSmall.h"
#include "TagBold.h"
#include "TagItalic.h"
#include "TagUnderline.h"
#include "TagBr.h"
#include "TagStrikeout.h"

namespace myodd{ namespace html{
Tag::Tag(const Attributes& attributes, int tagType) :
  _depth(0),
  _attributes(attributes),
  _tagType(tagType)
{
}

Tag::Tag(const Tag& tag)
{
  *this = tag;
}

Tag& Tag::operator=(const Tag& tag)
{
  if (this != &tag)
  {
    _depth = tag._depth;
    _attributes = tag._attributes;
    _tagType = tag._tagType;
  }
  return *this;
}

bool Tag::Is(Type type) const
{
  return (_tagType & type) != 0;
}

// apply the style
void Tag::Push(HDC hdc, LOGFONT& logFont )
{
  // first push the tag itself
  OnPush( hdc, logFont );

  // then the attributes
  _attributes.Push(hdc, logFont);

  ++_depth;
}

// remove the style
void Tag::Pop(HDC hdc, LOGFONT& logFont )
{
  if( _depth >= 1 )
  {
    // pop the attributes
    assert(0);  // we need to pop the parent attribute
                // a closing tag does not have attributes to pop!

    OnPop(hdc, logFont );
    --_depth;
  }
  else
  {
   // *** Trying to pop an un open token ***
  }
}

/**
 * \brief given a source tag, make a copy of it.
 * \param const Tag& the tag we are trying to create from.
 */
Tag* Tag::CreateFromSource(const Tag& src)
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
 * \param const int& the type flag, (open/closed etc)
 * \return Tag* or nullptr if the tag cannot be created
 */
Tag* Tag::CreateFromString(const std::wstring& tagName, const Attributes& attributes, const int& tagType)
{
  auto nLen = tagName.length();
  auto lowerTagName = myodd::strings::lower(tagName);
  auto lpString = lowerTagName.c_str();
  switch (nLen)
  {
  case 1:
    if (_tcsncmp(lpString, L"b", nLen) == 0)
    {
      return new TagBold(attributes, tagType);
    }
    if (_tcsncmp(lpString, L"i", nLen) == 0)
    {
      return new TagItalic(attributes, tagType);
    }
    if (_tcsncmp(lpString, L"s", nLen) == 0)
    {
      return new TagStrikeout(attributes, tagType);
    }
    if (_tcsncmp(lpString, L"u", nLen) == 0)
    {
      return new TagUnderline(attributes, tagType);
    }
    break;

  case 2:
    if (_tcsncmp(lpString, L"br", nLen) == 0)
    {
      return new TagBr(attributes, tagType);
    }
    if (_tcsncmp(lpString, L"em", nLen) == 0)
    {
      return new TagItalic(attributes, tagType);
    }
    break;

  case 5:
    if (_tcsncmp(lpString, L"small", nLen) == 0)
    {
      return new TagSmall(attributes, tagType);
    }
    break;

  case 6:
    if (_tcsncmp(lpString, L"strong", nLen) == 0)
    {
      return new TagBold(attributes, tagType);
    }
    break;

  case 9:
    if (_tcsncmp(lpString, L"strikeout", nLen) == 0)
    {
      return new TagStrikeout(attributes, tagType);
    }
    break;

  default:
    return false;
  }
  return false;
}
}}