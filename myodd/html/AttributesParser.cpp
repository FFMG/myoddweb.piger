#include "AttributesParser.h"
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
  Attributes atts;
  if (nullptr == lpString)
  {
    return atts;
  }

  auto totalLen = _tcslen(lpString);
  auto position = 0;
  // here we look for the name
  for (; position < totalLen;)
  {
    auto current = lpString[position];
    auto next = lpString[position+1];

    // move forward
    ++position;
  }
  
  return atts;
}
}}