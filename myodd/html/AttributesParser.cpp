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
 * \return
 */
Attributes AttributesParser::Parse(const std::wstring& text)
{
  return Parse(text.c_str());
}

/**
 * \brief Parse a string and build an array of HTML attributes and so on.
 * \param const std::wstring the string we are parsing.
 * \return
 */
Attributes AttributesParser::Parse(const wchar_t* lpString )
{
  Attributes atts;
  return atts;
}
}}