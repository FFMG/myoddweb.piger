#include "SimpleHtmlData.h"

namespace myodd { namespace html {
SimpleHtmlData::SimpleHtmlData(const std::wstring& text) :
  HtmlData( text, nullptr)
{
}
}}