#include "DomObjectClosingTag.h"

namespace myodd { namespace html {
DomObjectClosingTag::DomObjectClosingTag(const Tag& tagData, Tag& tagParent) :
  DomObjectTag( tagData ),
  _tagParent( tagParent )
{
}

DomObjectClosingTag::DomObjectClosingTag(const DomObjectClosingTag& rhs) :
  DomObjectTag( rhs ),
  _tagParent(rhs._tagParent)
{
}

DomObjectClosingTag::~DomObjectClosingTag()
{
}

// apply the style
void DomObjectClosingTag::Push(HDC hdc, LOGFONT& logFont)
{
  _tagParent.Push(hdc, logFont);
  DomObjectTag::Push(hdc, logFont);
}

// remove the style
void DomObjectClosingTag::Pop(HDC hdc, LOGFONT& logFont)
{
  DomObjectTag::Pop(hdc, logFont);
  _tagParent.Pop(hdc, logFont);
}
}}