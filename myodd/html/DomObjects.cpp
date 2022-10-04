#include "DomObjects.h"
#include "DomObjectTag.h"
#include "DomObjectClosingTag.h"
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
  //  if this is the closing tag then we need to look for the opening tag.
  // but if it is an opneing/closing tag then we do not need to worry about 
  // looking for a parent, (for example <img /> or <br />
  if (src.Is(Tag::Closing) && !src.Is(Tag::Opening))
  {
    auto parent = FindOpeningTag(src.TagType());
    if (nullptr != parent)
    {
      push_back(new DomObjectClosingTag(src, parent->TagData() ));
      return;
    }

    // if we could not find a parent, just add it to the list.
  }

  // this is an opening tag
  push_back(new DomObjectTag(src));
}

void DomObjects::AddContent(const std::wstring& content)
{
  push_back(new DomObjectContent(EscapeText(content)));
}

/**
 * \brief given a closing chain, look for the opening chain.
 * \param Tag::Type the parent tag we are looking for.
 * \return DomObject* the parent object or nullptr if we could not find one.
 */
DomObjectTag* DomObjects::FindOpeningTag(Tag::Type tagType ) const
{
  // we want to check in reverse for the last open one
  // that is not already closed.
  // in case we have something like <i>...<i>...</i></i> then the second </i> is not the one we want.
  auto skipTag = 0;
  for (auto it = rbegin(); it != rend(); it++)
  {
    auto tag = dynamic_cast<DomObjectTag*>(*it);
    if (tag == nullptr)
    {
      // it is not a tag
      continue;
    }

    if (!tag->TagData().Is(tagType))
    {
      // not the tag we are looking for.
      continue;
    }

    if (tag->TagData().Is(Tag::Closing))
    {
      // this is another closing tag
      // so we must look for its opening tag.
      ++skipTag;
      continue;
    }

    // this is the tag we are looking for
    // but are we skipping it?
    if (skipTag > 0)
    {
      --skipTag;
      continue;
    }

    // this is our opening tag!
    return tag;
  }

  // could not find the parent
  return nullptr;
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