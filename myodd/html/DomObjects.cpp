#include "DomObjects.h"
#include "SimpleHtmlData.h"

namespace myodd { namespace html {
DomObjects::~DomObjects()
{
  Clear();
}

DomObjects::DomObjects(const DomObjects& rhs)
{
  *this = rhs;
}

DomObjects& DomObjects::operator=(const DomObjects& rhs)
{
  if (this != &rhs)
  {
    Clear();
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
    {
      if ((*it)->IsHtmlTag())
      {
        AddTag((*it)->TagData());
      }
      else
      {
        AddContent((*it)->Text());
      }
    }
  }
  return *this;
}


/**
 * \brief delete all the DomObjects and clear the array.
 */
void DomObjects::Clear()
{
  for (auto it = begin(); it != end(); ++it)
  {
    delete* it;
  }
  clear();
}

void DomObjects::AddTag(const Tag& src)
{
  push_back(new DomObject(src));
}

void DomObjects::AddContent(const std::wstring& content)
{
  push_back(new SimpleHtmlData(EscapeText(content)));
}

/**
 * \brief Escape a given text
 * \param std::wstring the text we want to excape
 * \return the escaped text
 */
std::wstring DomObjects::EscapeText(const std::wstring& src) const
{
  auto text = src;
  text = myodd::strings::Replace(text, L"&nbsp;", L" ", false);
  text = myodd::strings::Replace(text, L"&lt;", L"<", false);
  text = myodd::strings::Replace(text, L"&gt;", L">", false);
  text = myodd::strings::Replace(text, L"&amp;", L"&", false);
  text = myodd::strings::Replace(text, L"&deg;", std::wstring(1, wchar_t(176)), false);    //  degree
  text = myodd::strings::Replace(text, L"&plusmn;", std::wstring(1, wchar_t(177)), false); //  Plus/minus symbol

  return text;
}


}}