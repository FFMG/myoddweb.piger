#include "AttributesParser.h"
#include "AttributeStyle.h"
#include "AttributeValueColor.h"

#include <algorithm>
#include "SimpleHtmlData.h"

namespace myodd { namespace html {
AttributesParser::AttributesParser( )
{
}

AttributesParser::~AttributesParser()
{
}

/**
 * \brief Parse a string and build an array of HTML attributes and so on.
 * \param const std::wstring the string we are parsing.
 * \return Attributes the attributes.
 */
Attributes AttributesParser::Parse(const std::wstring& text) const
{
  return Parse(text.c_str());
}

/**
 * \brief Parse a string and build an array of attributes.
 * \param const std::wstring the string we are parsing.
 * \return Attributes the attributes.
 */
Attributes AttributesParser::Parse(const wchar_t* lpString ) const
{
  auto totalLen = lpString == nullptr ? 0 : _tcslen(lpString);
  if (totalLen == 0 )
  {
    return Attributes();
  }

  Attributes atts;
  auto position = 0;

  // at the begining we have nothing
  std::wstring name = L"";
  std::wstring value = L"";

  // get the quote types
  auto insideSingleQuote = false;
  auto insideDoubleQuote = false;

  // if we have a name we can look for a value.
  auto hasName = false;

  for (; position < totalLen;)
  {
    const auto& current = lpString[position];
    const auto& next = position < totalLen ? lpString[position+1] : L'\0';

    if (false == hasName)
    {
      name += current;
      if (L'=' == next )
      {
        hasName = true;

        // move forward so we can ignore the equal sign
        ++position;
      }
      if (L'\0' == next)
      {
        // we cannot use this value
        // we only have a name but no value.
        break;
      }

      // move forward so we can move to the next character.
      ++position;
      continue;
    }

    // double quote
    if( current == L'"')
    {
      // we are inside a doublt quote 
      // so we are now getting out.
      if (insideDoubleQuote)
      {
        auto attribute = CreateAttribute(name, value);
        if (nullptr != attribute)
        {
          atts.Add(*attribute);
          delete attribute;
        }
        insideDoubleQuote = insideSingleQuote = hasName = false;
        name = value = L"";

        ++position;
        continue;
      }
      if (insideSingleQuote)
      {
        // we are inside a single quote 
        // so the double quote is part of what we are using.
        value += current;

        ++position;
        continue;
      }
      
      // we are neither inside a double or single
      // so we are now inside a double.
      insideDoubleQuote = true;

      ++position;
      continue;
    }

    // single quote
    if (current == L'\'')
    {
      // we are inside a single quote 
      // so we are now getting out.
      if (insideSingleQuote)
      {
        auto attribute = CreateAttribute(name, value);
        if (nullptr != attribute)
        {
          atts.Add(*attribute);
          delete attribute;
        }
        insideDoubleQuote = insideSingleQuote = hasName = false;
        name = value = L"";

        ++position;
        continue;
      }
      if (insideDoubleQuote)
      {
        // we are inside a single quote 
        // so the double quote is part of what we are using.
        value += current;

        ++position;
        continue;
      }

      // we are neither inside a double or single
      // so we are now inside a single.
      insideSingleQuote = true;

      ++position;
      continue;
    }

    // if we are neither inside a single or double quote ... then I have no idea what this is for
    // and it is probably not valid and needs to be ignored.
    if (!insideDoubleQuote && !insideSingleQuote)
    {
      ++position;
      continue;
    }

    // we are inside either single or double
    // so we can move forward with life
    value += current;
    ++position;
  }
  
  return atts;
}

Attribute* AttributesParser::CreateAttribute(const std::wstring& name, const std::wstring& value) const
{
  // try and find the type
  const auto lowerName = myodd::strings::lower(name);
  const auto givenName = lowerName.c_str();
  const auto givenNameLength = lowerName.length();

  switch (givenNameLength)
  {
  case 5:
    if (_tcsnicmp(givenName, L"style", givenNameLength) == 0)
    {
      auto style = new AttributeStyle();
      style->Add(AttributeValueColor() );
      return style;
    }
  }

  // not sure what this is.
  return nullptr;
}
}}