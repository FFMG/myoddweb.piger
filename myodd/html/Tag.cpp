#include "..\string\string.h"
#include "Tag.h"
#include "TagSmall.h"
#include "TagBold.h"
#include "TagItalic.h"
#include "TagUnderline.h"
#include "TagBr.h"
#include "TagStrikeout.h"
#include "TagSpan.h"

namespace myodd{ namespace html{
Tag::Tag(const Attributes& attributes, int tagStyle) :
  _depth(0),
  _attributes(attributes),
  _tagStyle(tagStyle)
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
    _tagStyle = tag._tagStyle;
  }
  return *this;
}

/**
 * \brief check if something of a given type.
 * \param const Type the type we are checking
 * \return bool if the flag we are checking is set
 */
bool Tag::Is(Type type) const
{
  return TagType() == type;
}

/**
 * \brief check if something of a given style.
 * \param const Style the style we are checking
 * \return bool if the style is the one we are looking for.
 */
bool Tag::Is(Style style) const
{
  return (_tagStyle & style) != 0;
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
    _attributes.Pop(hdc, logFont);

    // then the main tag style
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
  switch (src.TagType())
  {
  case Bold:
    {
      auto tagBold = dynamic_cast<const TagBold*>(&src);
      if (nullptr != tagBold)
      {
        return new TagBold(*tagBold);
      }
    }
    break;

  case Br:
    {
      auto tagBr = dynamic_cast<const TagBr*>(&src);
      if (nullptr != tagBr)
      {
        return new TagBr(*tagBr);
      }
    }
    break;

  case Italic:
    {
      auto tagItalic = dynamic_cast<const TagItalic*>(&src);
      if (nullptr != tagItalic)
      {
        return new TagItalic(*tagItalic);
      }
    }
    break;

  case Small:
    {
      auto tagSmall = dynamic_cast<const TagSmall*>(&src);
      if (nullptr != tagSmall)
      {
        return new TagSmall(*tagSmall);
      }
    }
    break;

  case Span:
  {
    auto tagSpan = dynamic_cast<const TagSpan*>(&src);
    if (nullptr != tagSpan)
    {
      return new TagSpan(*tagSpan);
    }
  }
  break;

  case StrikeOut:
    {
      auto tagStrikeout = dynamic_cast<const TagStrikeout*>(&src);
      if (nullptr != tagStrikeout)
      {
        return new TagStrikeout(*tagStrikeout);
      }
    }
    break;

  case UnderLine:
    {
      auto tagUnderline = dynamic_cast<const TagUnderline*>(&src);
      if (nullptr != tagUnderline)
      {
        return new TagUnderline(*tagUnderline);
      }
    }
    break;

  default:
    break;
  }

  assert(0);  //  the kind does not match any known type
              // or could not be cast down
  return nullptr;
}

/**
 * \brief create a tag given the tag name.
 * \param const std::wstring& the tag name we will be looking for.
 * \param const Attributes& the tag attributes
 * \param const int& the style flag, (open/closed etc)
 * \return Tag* or nullptr if the tag cannot be created
 */
Tag* Tag::CreateFromString(const std::wstring& tagName, const Attributes& attributes, const int& tagStyle)
{
  auto nLen = tagName.length();
  auto lowerTagName = myodd::strings::lower(tagName);
  auto lpString = lowerTagName.c_str();
  switch (nLen)
  {
  case 1:
    if (_tcsncmp(lpString, L"b", nLen) == 0)
    {
      return new TagBold(attributes, tagStyle);
    }
    if (_tcsncmp(lpString, L"i", nLen) == 0)
    {
      return new TagItalic(attributes, tagStyle);
    }
    if (_tcsncmp(lpString, L"s", nLen) == 0)
    {
      return new TagStrikeout(attributes, tagStyle);
    }
    if (_tcsncmp(lpString, L"u", nLen) == 0)
    {
      return new TagUnderline(attributes, tagStyle);
    }
    break;

  case 2:
    if (_tcsncmp(lpString, L"br", nLen) == 0)
    {
      return new TagBr(attributes, tagStyle);
    }
    if (_tcsncmp(lpString, L"em", nLen) == 0)
    {
      return new TagItalic(attributes, tagStyle);
    }
    break;

  case 4:
    if (_tcsncmp(lpString, L"span", nLen) == 0)
    {
      return new TagSpan(attributes, tagStyle);
    }
    break;

  case 5:
    if (_tcsncmp(lpString, L"small", nLen) == 0)
    {
      return new TagSmall(attributes, tagStyle);
    }
    break;

  case 6:
    if (_tcsncmp(lpString, L"strong", nLen) == 0)
    {
      return new TagBold(attributes, tagStyle);
    }
    break;

  case 9:
    if (_tcsncmp(lpString, L"strikeout", nLen) == 0)
    {
      return new TagStrikeout(attributes, tagStyle);
    }
    break;

  default:
    return false;
  }
  return false;
}
}}