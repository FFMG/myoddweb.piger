#pragma once
#include "../string/string.h"
#include "DomObject.h"

namespace myodd { namespace html {
class DomObjectContent : public DomObject
{

public:

  DomObjectContent(const std::wstring& text);
  DomObjectContent(const DomObjectContent& rhs );
  DomObjectContent& operator=(const DomObjectContent& rhs);

  virtual ~DomObjectContent() = default;

  // helpers

  /**
   * \brief get the length of text
   * \return int the length
   */
  const unsigned int TextLength() const;

  /**
   * \brief the text/content of this object
   * \return const std::wstring& the text
   */
  const std::wstring& Text() const;

private:
  /**
   * \brief the content of the object
   */
  std::wstring _text;
};
}}
