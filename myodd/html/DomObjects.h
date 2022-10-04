#pragma once
#include <vector>
#include "Tag.h"
#include "DomObject.h"
#include "DomObjectTag.h"

namespace myodd { namespace html {
class DomObjects : public std::vector<DomObject*>
{
public:
  DomObjects() = default;

  DomObjects( const DomObjects& );
  DomObjects& operator=( const DomObjects& );

  /**
   * \brief delete all the DomObjects and clear the array.
   */
  virtual ~DomObjects();

  void AddTag(const Tag& src);
  void AddContent(const std::wstring& content);
  void Clear();

private:
  std::wstring EscapeText(const std::wstring& src) const;

  /**
   * \brief given a closing chain, look for the opening chain.
   * \param Tag::Type the parent tag we are looking for.
   * \return DomObject* the parent object or nullptr if we could not find one.
   */
  DomObjectTag* FindOpeningTag(Tag::Type tagType ) const;
};
}}
