#pragma once
#include "../string/string.h"
#include "DomObject.h"

namespace myodd { namespace html {
class DomObjectContent : public DomObject
{

public:

  DomObjectContent(const std::wstring& text);
  DomObjectContent(const DomObjectContent& rhs );
  const DomObjectContent& operator=(const DomObjectContent& rhs) = delete;

  virtual ~DomObjectContent() = default;

  // helpers
  const int TextLength() const;
  const std::wstring& Text() const;

  // apply the style
  void Push(HDC hdc, LOGFONT& logFont);

  // remove the style
  void Pop(HDC hdc, LOGFONT& logFont);

protected:
  const std::wstring _text;
};
}}
