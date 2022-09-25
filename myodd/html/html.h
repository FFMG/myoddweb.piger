#pragma once
/* DrawHTML2()
* Drop-in replacement for DrawText() supporting a tiny subset of HTML.
*/

namespace myodd{ namespace html{
  SIZE html(HDC     hdc,        // handle of device context
            const wchar_t* lpString,   // address of string to draw
            int     nCount,     // string length, in characters
            LPRECT  lpRect,     // address of structure with formatting dimensions
            const int maxLineHeight,
            const int paddingTop,
            const int paddingBottom,
            UINT    uFormat     // text-drawing flags
            );
  } //  html
} //  myodd
