#include "DomObjects.h"
#include "DomObjectTag.h"
#include "DomObjectContent.h"

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
      auto isTag = dynamic_cast<DomObjectTag*>(*it);
      if ( isTag != nullptr )
      {
        AddTag(isTag->TagData());
        continue;
      }

      auto isContent = dynamic_cast<DomObjectContent*>(*it);
      if (isContent != nullptr)
      {
        AddContent(isContent->Text());
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
  push_back(new DomObjectTag(src));
}

void DomObjects::AddContent(const std::wstring& content)
{
  push_back(new DomObjectContent(EscapeText(content)));
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